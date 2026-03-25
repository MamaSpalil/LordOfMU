#include "stdafx.h"
#include "D3D9Hook.h"
#include "ClickerLogger.h"
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

// ---------------------------------------------------------------------------
// Typedefs for original D3D9 methods
// ---------------------------------------------------------------------------
typedef HRESULT(WINAPI* FnEndScene)(IDirect3DDevice9*);
typedef HRESULT(WINAPI* FnReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

// ---------------------------------------------------------------------------
// Module state
// ---------------------------------------------------------------------------
namespace
{
	// Original method pointers (restored on uninstall).
	FnEndScene  g_pOrigEndScene  = NULL;
	FnReset     g_pOrigReset     = NULL;

	// Vtable entry addresses so we can write-protect / restore.
	void**      g_ppVtEndScene   = NULL;
	void**      g_ppVtReset      = NULL;

	// D3D9Ex devices (created via Direct3DCreate9Ex) use a separate vtable
	// from regular D3D9 HAL devices.  Many modern MU Online clients and
	// Windows 10/11 runtimes use D3D9Ex internally, so we must also patch
	// the Ex vtable to ensure the EndScene hook fires.
	FnEndScene  g_pOrigEndSceneEx  = NULL;
	FnReset     g_pOrigResetEx     = NULL;
	void**      g_ppVtEndSceneEx   = NULL;
	void**      g_ppVtResetEx      = NULL;

	// User callbacks.
	FnOnEndScene  g_pfnOnEndScene  = NULL;
	FnOnPreReset  g_pfnOnPreReset  = NULL;
	FnOnPostReset g_pfnOnPostReset = NULL;

	// Captured game device.
	IDirect3DDevice9* g_pGameDevice = NULL;

	// Guard against re-entrant EndScene calls.
	// NOTE: EndScene is only called from the D3D rendering thread, so
	// a simple boolean guard suffices (no atomic/interlock needed).
	volatile bool g_bInEndScene = false;

	// Installed flag.
	bool g_bInstalled = false;
}

// ---------------------------------------------------------------------------
// Helper: overwrite a vtable entry with DWORD protection dance.
// ---------------------------------------------------------------------------
static BOOL PatchVtableEntry(void** ppEntry, void* pNewFunc, void** ppOldFunc)
{
	DWORD dwOldProtect = 0;
	if (!VirtualProtect(ppEntry, sizeof(void*), PAGE_EXECUTE_READWRITE, &dwOldProtect))
		return FALSE;

	if (ppOldFunc)
		*ppOldFunc = *ppEntry;

	*ppEntry = pNewFunc;

	DWORD dwDummy = 0;
	VirtualProtect(ppEntry, sizeof(void*), dwOldProtect, &dwDummy);
	return TRUE;
}

// ---------------------------------------------------------------------------
// Hooked EndScene
// ---------------------------------------------------------------------------
static HRESULT WINAPI HookedEndScene(IDirect3DDevice9* pDevice)
{
	if (!g_bInEndScene)
	{
		g_bInEndScene = true;
		g_pGameDevice = pDevice;

		if (g_pfnOnEndScene)
			g_pfnOnEndScene(pDevice);

		g_bInEndScene = false;
	}

	return g_pOrigEndScene(pDevice);
}

// ---------------------------------------------------------------------------
// Hooked Reset
// ---------------------------------------------------------------------------
static HRESULT WINAPI HookedReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPP)
{
	if (g_pfnOnPreReset)
		g_pfnOnPreReset(pDevice, pPP);

	HRESULT hr = g_pOrigReset(pDevice, pPP);

	if (g_pfnOnPostReset)
		g_pfnOnPostReset(pDevice, pPP, hr);

	return hr;
}

// ---------------------------------------------------------------------------
// Hooked EndScene for D3D9Ex devices (separate vtable from regular D3D9).
// ---------------------------------------------------------------------------
static HRESULT WINAPI HookedEndSceneEx(IDirect3DDevice9* pDevice)
{
	if (!g_bInEndScene)
	{
		g_bInEndScene = true;
		g_pGameDevice = pDevice;

		if (g_pfnOnEndScene)
			g_pfnOnEndScene(pDevice);

		g_bInEndScene = false;
	}

	return g_pOrigEndSceneEx(pDevice);
}

// ---------------------------------------------------------------------------
// Hooked Reset for D3D9Ex devices.
// ---------------------------------------------------------------------------
static HRESULT WINAPI HookedResetEx(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPP)
{
	if (g_pfnOnPreReset)
		g_pfnOnPreReset(pDevice, pPP);

	HRESULT hr = g_pOrigResetEx(pDevice, pPP);

	if (g_pfnOnPostReset)
		g_pfnOnPostReset(pDevice, pPP, hr);

	return hr;
}

// ---------------------------------------------------------------------------
// Helper: create (and later destroy) a temporary hidden window for the
// D3D9 dummy device.  Using a dedicated hidden window avoids conflicts
// with the game's own HWND — in exclusive fullscreen mode the game window
// may reject additional D3D device creation attempts.
// ---------------------------------------------------------------------------
static const char* s_szDummyWndClass = "D3D9Hook_DummyWnd";

static HWND CreateDummyWindow()
{
	WNDCLASSEXA wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize        = sizeof(wc);
	wc.style         = CS_CLASSDC;
	wc.lpfnWndProc   = DefWindowProcA;
	wc.hInstance     = GetModuleHandle(NULL);
	wc.lpszClassName = s_szDummyWndClass;
	RegisterClassExA(&wc);

	return CreateWindowExA(0, s_szDummyWndClass, NULL, WS_OVERLAPPEDWINDOW,
		0, 0, 100, 100, NULL, NULL, wc.hInstance, NULL);
}

static void DestroyDummyWindow(HWND hWnd)
{
	if (hWnd)
		DestroyWindow(hWnd);
	UnregisterClassA(s_szDummyWndClass, GetModuleHandle(NULL));
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------
BOOL D3D9Hook::Install(HWND hwndGame)
{
	if (g_bInstalled)
		return TRUE;

	// -----------------------------------------------------------------------
	// Step 0: Create a temporary hidden window for the dummy D3D devices.
	// In exclusive fullscreen the game's HWND may be incompatible with
	// additional windowed device creation.  A separate hidden window avoids
	// this problem — the vtable is the same regardless of the HWND used.
	// -----------------------------------------------------------------------
	HWND hDummyWnd = CreateDummyWindow();
	if (!hDummyWnd)
	{
		// Fall back to the game window if the helper window can't be created.
		hDummyWnd = hwndGame;
		WriteClickerLogFmt("D3D9HOOK", "CreateDummyWindow failed, falling back to game HWND 0x%p", hwndGame);
	}

	// -----------------------------------------------------------------------
	// Step 1: Load d3d9.dll and create a temporary device to get vtable.
	// -----------------------------------------------------------------------
	HMODULE hD3D9 = GetModuleHandleA("d3d9.dll");
	if (!hD3D9)
		hD3D9 = LoadLibraryA("d3d9.dll");
	if (!hD3D9)
	{
		WriteClickerLogFmt("D3D9HOOK", "d3d9.dll not found (GetModuleHandle + LoadLibrary failed)");
		DestroyDummyWindow(hDummyWnd == hwndGame ? NULL : hDummyWnd);
		return FALSE;
	}

	typedef IDirect3D9* (WINAPI* FnDirect3DCreate9)(UINT);
	FnDirect3DCreate9 pfnCreate9 =
		(FnDirect3DCreate9)GetProcAddress(hD3D9, "Direct3DCreate9");
	if (!pfnCreate9)
	{
		WriteClickerLogFmt("D3D9HOOK", "Direct3DCreate9 not found in d3d9.dll");
		DestroyDummyWindow(hDummyWnd == hwndGame ? NULL : hDummyWnd);
		return FALSE;
	}

	IDirect3D9* pD3D = pfnCreate9(D3D_SDK_VERSION);
	if (!pD3D)
	{
		WriteClickerLogFmt("D3D9HOOK", "Direct3DCreate9 returned NULL");
		DestroyDummyWindow(hDummyWnd == hwndGame ? NULL : hDummyWnd);
		return FALSE;
	}

	// Use a minimal present-parameters set for the dummy device.
	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed            = TRUE;
	pp.SwapEffect          = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow       = hDummyWnd;
	pp.BackBufferFormat    = D3DFMT_UNKNOWN;
	pp.BackBufferCount     = 1;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	IDirect3DDevice9* pDummy = NULL;

	// Use D3DDEVTYPE_HAL (same device type as the game) so that the dummy
	// device shares the same vtable as the game's real device.  NULLREF
	// devices have a separate vtable on modern Windows 10/11, so patching
	// the NULLREF vtable does not intercept calls on a HAL device.
	HRESULT hr = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hDummyWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&pp,
		&pDummy);

	if (FAILED(hr) || !pDummy)
	{
		WriteClickerLogFmt("D3D9HOOK", "CreateDevice(HAL) failed: hr=0x%08X, hWnd=0x%p", hr, hDummyWnd);
		pD3D->Release();
		DestroyDummyWindow(hDummyWnd == hwndGame ? NULL : hDummyWnd);
		return FALSE;
	}

	// -----------------------------------------------------------------------
	// Step 2: Read the vtable from the dummy device.
	// IDirect3DDevice9 vtable layout (partial):
	//   [0]  QueryInterface
	//   ...
	//   [16] Reset
	//   ...
	//   [42] EndScene
	// -----------------------------------------------------------------------
	void** ppVtable = *(void***)pDummy;

	g_ppVtEndScene = &ppVtable[42];
	g_ppVtReset    = &ppVtable[16];

	// Release the dummy — we only needed the vtable pointer.
	// HAL devices in the same process share the same vtable (same d3d9.dll
	// code), so patching the dummy HAL vtable patches the game's HAL device.
	pDummy->Release();
	pD3D->Release();

	// -----------------------------------------------------------------------
	// Step 3: Patch EndScene and Reset.
	// -----------------------------------------------------------------------
	if (!PatchVtableEntry(g_ppVtEndScene, (void*)HookedEndScene, (void**)&g_pOrigEndScene))
	{
		WriteClickerLogFmt("D3D9HOOK", "PatchVtableEntry(EndScene) failed");
		DestroyDummyWindow(hDummyWnd == hwndGame ? NULL : hDummyWnd);
		return FALSE;
	}

	if (!PatchVtableEntry(g_ppVtReset, (void*)HookedReset, (void**)&g_pOrigReset))
	{
		WriteClickerLogFmt("D3D9HOOK", "PatchVtableEntry(Reset) failed, rolling back EndScene");
		// Rollback EndScene if Reset patch fails.
		PatchVtableEntry(g_ppVtEndScene, (void*)g_pOrigEndScene, NULL);
		g_pOrigEndScene = NULL;
		DestroyDummyWindow(hDummyWnd == hwndGame ? NULL : hDummyWnd);
		return FALSE;
	}

	g_bInstalled = true;

	WriteClickerLogFmt("D3D9HOOK", "Step 3 OK: Regular D3D9 vtable patched (EndScene=0x%p, Reset=0x%p)",
		ppVtable[42], ppVtable[16]);

	// -----------------------------------------------------------------------
	// Step 4: Also patch the D3D9Ex vtable (if available).
	// Games or the Windows runtime may create the D3D device via
	// Direct3DCreate9Ex + CreateDeviceEx, which produces an
	// IDirect3DDevice9Ex with a DIFFERENT vtable from regular
	// IDirect3DDevice9 HAL devices.  Without this extra patch the
	// EndScene hook never fires on D3D9Ex games (including most games
	// running on Windows 10/11 where the runtime internally promotes
	// devices to D3D9Ex).
	//
	// IMPORTANT: We must use CreateDeviceEx() (not CreateDevice()) on the
	// IDirect3D9Ex interface to obtain the true D3D9Ex device vtable.
	// IDirect3D9Ex::CreateDevice() still creates a base IDirect3DDevice9
	// whose vtable matches the regular D3D9 one — patching it has no
	// additional effect.
	// -----------------------------------------------------------------------
#if !defined(D3D_DISABLE_9EX)
	{
		typedef HRESULT(WINAPI* FnDirect3DCreate9Ex)(UINT, IDirect3D9Ex**);
		FnDirect3DCreate9Ex pfnCreate9Ex =
			(FnDirect3DCreate9Ex)GetProcAddress(hD3D9, "Direct3DCreate9Ex");

		if (pfnCreate9Ex)
		{
			IDirect3D9Ex* pD3DEx = NULL;
			if (SUCCEEDED(pfnCreate9Ex(D3D_SDK_VERSION, &pD3DEx)))
			{
				D3DPRESENT_PARAMETERS ppEx;
				ZeroMemory(&ppEx, sizeof(ppEx));
				ppEx.Windowed             = TRUE;
				ppEx.SwapEffect           = D3DSWAPEFFECT_DISCARD;
				ppEx.hDeviceWindow        = hDummyWnd;
				ppEx.BackBufferFormat     = D3DFMT_UNKNOWN;
				ppEx.BackBufferCount      = 1;
				ppEx.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

				IDirect3DDevice9Ex* pDummyEx = NULL;
				HRESULT hrEx = pD3DEx->CreateDeviceEx(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL,
					hDummyWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
					&ppEx,
					NULL,       // pFullscreenDisplayMode — NULL for windowed
					&pDummyEx);

				if (SUCCEEDED(hrEx) && pDummyEx)
				{
					void** ppVtableEx = *(void***)pDummyEx;

					// Only patch if the Ex vtable differs from the regular one.
					if (ppVtableEx != ppVtable)
					{
						g_ppVtEndSceneEx = &ppVtableEx[42];
						g_ppVtResetEx    = &ppVtableEx[16];

						PatchVtableEntry(g_ppVtEndSceneEx, (void*)HookedEndSceneEx,
							(void**)&g_pOrigEndSceneEx);

						PatchVtableEntry(g_ppVtResetEx, (void*)HookedResetEx,
							(void**)&g_pOrigResetEx);

						WriteClickerLogFmt("D3D9HOOK", "Step 4 OK: D3D9Ex vtable patched (EndSceneEx=0x%p, ppVtableEx=0x%p)",
							ppVtableEx[42], ppVtableEx);
					}
					else
					{
						WriteClickerLogFmt("D3D9HOOK", "Step 4: D3D9Ex vtable == regular vtable (0x%p), Ex patch skipped",
							ppVtable);
					}

					pDummyEx->Release();
				}
				else
				{
					WriteClickerLogFmt("D3D9HOOK", "Step 4: CreateDeviceEx failed: hr=0x%08X (D3D9Ex patch skipped)", hrEx);
				}

				pD3DEx->Release();
			}
			else
			{
				WriteClickerLogFmt("D3D9HOOK", "Step 4: Direct3DCreate9Ex failed (D3D9Ex patch skipped)");
			}
		}
		else
		{
			WriteClickerLogFmt("D3D9HOOK", "Step 4: Direct3DCreate9Ex not found in d3d9.dll (D3D9Ex not available)");
		}
	}
#endif // !D3D_DISABLE_9EX

	// Clean up the temporary helper window.
	DestroyDummyWindow(hDummyWnd == hwndGame ? NULL : hDummyWnd);

	return TRUE;
}

void D3D9Hook::Uninstall()
{
	if (!g_bInstalled)
		return;

	// Restore regular D3D9 vtable.
	if (g_ppVtEndScene && g_pOrigEndScene)
		PatchVtableEntry(g_ppVtEndScene, (void*)g_pOrigEndScene, NULL);

	if (g_ppVtReset && g_pOrigReset)
		PatchVtableEntry(g_ppVtReset, (void*)g_pOrigReset, NULL);

	// Restore D3D9Ex vtable (if it was patched).
	if (g_ppVtEndSceneEx && g_pOrigEndSceneEx)
		PatchVtableEntry(g_ppVtEndSceneEx, (void*)g_pOrigEndSceneEx, NULL);

	if (g_ppVtResetEx && g_pOrigResetEx)
		PatchVtableEntry(g_ppVtResetEx, (void*)g_pOrigResetEx, NULL);

	g_pOrigEndScene    = NULL;
	g_pOrigReset       = NULL;
	g_ppVtEndScene     = NULL;
	g_ppVtReset        = NULL;
	g_pOrigEndSceneEx  = NULL;
	g_pOrigResetEx     = NULL;
	g_ppVtEndSceneEx   = NULL;
	g_ppVtResetEx      = NULL;
	g_pGameDevice      = NULL;
	g_pfnOnEndScene    = NULL;
	g_pfnOnPreReset    = NULL;
	g_pfnOnPostReset   = NULL;
	g_bInstalled       = false;
}

void D3D9Hook::SetOnEndScene(FnOnEndScene pfn)   { g_pfnOnEndScene  = pfn; }
void D3D9Hook::SetOnPreReset(FnOnPreReset pfn)   { g_pfnOnPreReset  = pfn; }
void D3D9Hook::SetOnPostReset(FnOnPostReset pfn) { g_pfnOnPostReset = pfn; }

IDirect3DDevice9* D3D9Hook::GetDevice()           { return g_pGameDevice; }

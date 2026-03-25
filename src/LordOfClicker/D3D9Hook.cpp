#include "stdafx.h"
#include "D3D9Hook.h"
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
// Public API
// ---------------------------------------------------------------------------
BOOL D3D9Hook::Install(HWND hwndGame)
{
	if (g_bInstalled)
		return TRUE;

	// -----------------------------------------------------------------------
	// Step 1: Load d3d9.dll and create a temporary device to get vtable.
	// -----------------------------------------------------------------------
	HMODULE hD3D9 = GetModuleHandleA("d3d9.dll");
	if (!hD3D9)
		hD3D9 = LoadLibraryA("d3d9.dll");
	if (!hD3D9)
		return FALSE;

	typedef IDirect3D9* (WINAPI* FnDirect3DCreate9)(UINT);
	FnDirect3DCreate9 pfnCreate9 =
		(FnDirect3DCreate9)GetProcAddress(hD3D9, "Direct3DCreate9");
	if (!pfnCreate9)
		return FALSE;

	IDirect3D9* pD3D = pfnCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return FALSE;

	// Use a minimal present-parameters set for the dummy device.
	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed            = TRUE;
	pp.SwapEffect          = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow       = hwndGame;
	pp.BackBufferFormat    = D3DFMT_UNKNOWN;
	pp.BackBufferCount     = 1;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	IDirect3DDevice9* pDummy = NULL;
	HRESULT hr = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,     // null-ref device — lightweight, no GPU
		hwndGame,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&pp,
		&pDummy);

	if (FAILED(hr) || !pDummy)
	{
		pD3D->Release();
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
	// All IDirect3DDevice9 objects in the same process share the same vtable
	// (same d3d9.dll code), so patching the dummy's vtable patches them all.
	pDummy->Release();
	pD3D->Release();

	// -----------------------------------------------------------------------
	// Step 3: Patch EndScene and Reset.
	// -----------------------------------------------------------------------
	if (!PatchVtableEntry(g_ppVtEndScene, (void*)HookedEndScene, (void**)&g_pOrigEndScene))
		return FALSE;

	if (!PatchVtableEntry(g_ppVtReset, (void*)HookedReset, (void**)&g_pOrigReset))
	{
		// Rollback EndScene if Reset patch fails.
		PatchVtableEntry(g_ppVtEndScene, (void*)g_pOrigEndScene, NULL);
		g_pOrigEndScene = NULL;
		return FALSE;
	}

	g_bInstalled = true;
	return TRUE;
}

void D3D9Hook::Uninstall()
{
	if (!g_bInstalled)
		return;

	if (g_ppVtEndScene && g_pOrigEndScene)
		PatchVtableEntry(g_ppVtEndScene, (void*)g_pOrigEndScene, NULL);

	if (g_ppVtReset && g_pOrigReset)
		PatchVtableEntry(g_ppVtReset, (void*)g_pOrigReset, NULL);

	g_pOrigEndScene  = NULL;
	g_pOrigReset     = NULL;
	g_ppVtEndScene   = NULL;
	g_ppVtReset      = NULL;
	g_pGameDevice    = NULL;
	g_pfnOnEndScene  = NULL;
	g_pfnOnPreReset  = NULL;
	g_pfnOnPostReset = NULL;
	g_bInstalled     = false;
}

void D3D9Hook::SetOnEndScene(FnOnEndScene pfn)   { g_pfnOnEndScene  = pfn; }
void D3D9Hook::SetOnPreReset(FnOnPreReset pfn)   { g_pfnOnPreReset  = pfn; }
void D3D9Hook::SetOnPostReset(FnOnPostReset pfn) { g_pfnOnPostReset = pfn; }

IDirect3DDevice9* D3D9Hook::GetDevice()           { return g_pGameDevice; }

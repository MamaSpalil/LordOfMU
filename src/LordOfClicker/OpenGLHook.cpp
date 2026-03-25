#include "stdafx.h"
#include "OpenGLHook.h"
#include "ClickerLogger.h"

#pragma comment(lib, "opengl32.lib")

// ---------------------------------------------------------------------------
// Typedefs
// ---------------------------------------------------------------------------
typedef BOOL (WINAPI* FnSwapBuffers)(HDC);

// ---------------------------------------------------------------------------
// Module state
// ---------------------------------------------------------------------------
namespace
{
	// Original SwapBuffers via trampoline.
	FnSwapBuffers g_pOrigSwapBuffers = NULL;

	// Address of the real SwapBuffers entry point in gdi32.dll.
	BYTE*         g_pSwapBuffersAddr = NULL;

	// Trampoline: saved original bytes + JMP back.
	BYTE*         g_pTrampoline      = NULL;

	// Saved original bytes (for Uninstall).
	static const int HOOK_SIZE       = 5;  // 5-byte JMP rel32
	BYTE          g_SavedBytes[HOOK_SIZE] = {};

	// User callback.
	FnOnSwapBuffers g_pfnOnSwapBuffers = NULL;

	// Re-entrant guard.
	volatile bool g_bInSwapBuffers = false;

	// Installed / fired flags.
	bool g_bInstalled = false;
	bool g_bFired     = false;
}

// ---------------------------------------------------------------------------
// Hooked SwapBuffers
// ---------------------------------------------------------------------------
static BOOL WINAPI HookedSwapBuffers(HDC hdc)
{
	if (!g_bInSwapBuffers)
	{
		g_bInSwapBuffers = true;
		g_bFired = true;

		if (g_pfnOnSwapBuffers)
			g_pfnOnSwapBuffers(hdc);

		g_bInSwapBuffers = false;
	}

	return g_pOrigSwapBuffers(hdc);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------
BOOL OpenGLHook::Install()
{
	if (g_bInstalled)
		return TRUE;

	// -----------------------------------------------------------------------
	// Step 1: Locate SwapBuffers in gdi32.dll.
	// -----------------------------------------------------------------------
	HMODULE hGdi32 = GetModuleHandleA("gdi32.dll");
	if (!hGdi32)
	{
		WriteClickerLogFmt("GLHOOK", "gdi32.dll not found");
		return FALSE;
	}

	g_pSwapBuffersAddr = (BYTE*)GetProcAddress(hGdi32, "SwapBuffers");
	if (!g_pSwapBuffersAddr)
	{
		WriteClickerLogFmt("GLHOOK", "SwapBuffers not found in gdi32.dll");
		return FALSE;
	}

	WriteClickerLogFmt("GLHOOK", "SwapBuffers found at 0x%p", g_pSwapBuffersAddr);

	// -----------------------------------------------------------------------
	// Step 2: Allocate executable trampoline memory.
	// The trampoline holds the first HOOK_SIZE bytes of the original function
	// followed by a JMP back to (SwapBuffers + HOOK_SIZE).
	// -----------------------------------------------------------------------
	g_pTrampoline = (BYTE*)VirtualAlloc(NULL, 32,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!g_pTrampoline)
	{
		WriteClickerLogFmt("GLHOOK", "VirtualAlloc for trampoline failed");
		return FALSE;
	}

	// Save original bytes.
	memcpy(g_SavedBytes, g_pSwapBuffersAddr, HOOK_SIZE);

	// Copy original prologue to trampoline.
	memcpy(g_pTrampoline, g_pSwapBuffersAddr, HOOK_SIZE);

	// Append JMP back to (original + HOOK_SIZE).
	g_pTrampoline[HOOK_SIZE] = 0xE9;  // JMP rel32
	DWORD dwJmpBack = (DWORD)(g_pSwapBuffersAddr + HOOK_SIZE)
		- (DWORD)(g_pTrampoline + HOOK_SIZE + HOOK_SIZE);
	memcpy(g_pTrampoline + HOOK_SIZE + 1, &dwJmpBack, sizeof(DWORD));

	// The trampoline IS the "original" SwapBuffers callable.
	g_pOrigSwapBuffers = (FnSwapBuffers)g_pTrampoline;

	// -----------------------------------------------------------------------
	// Step 3: Overwrite the first 5 bytes of SwapBuffers with a JMP to
	// our HookedSwapBuffers function.
	// -----------------------------------------------------------------------
	DWORD dwOldProtect = 0;
	if (!VirtualProtect(g_pSwapBuffersAddr, HOOK_SIZE,
		PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		WriteClickerLogFmt("GLHOOK", "VirtualProtect on SwapBuffers failed");
		VirtualFree(g_pTrampoline, 0, MEM_RELEASE);
		g_pTrampoline = NULL;
		return FALSE;
	}

	g_pSwapBuffersAddr[0] = 0xE9;  // JMP rel32
	DWORD dwJmpHook = (DWORD)HookedSwapBuffers
		- (DWORD)(g_pSwapBuffersAddr + HOOK_SIZE);
	memcpy(g_pSwapBuffersAddr + 1, &dwJmpHook, sizeof(DWORD));

	DWORD dwDummy = 0;
	VirtualProtect(g_pSwapBuffersAddr, HOOK_SIZE, dwOldProtect, &dwDummy);

	g_bInstalled = true;

	WriteClickerLogFmt("GLHOOK", "SwapBuffers hook installed (trampoline=0x%p, hook=0x%p)",
		g_pTrampoline, HookedSwapBuffers);

	return TRUE;
}

void OpenGLHook::Uninstall()
{
	if (!g_bInstalled)
		return;

	// Restore original bytes.
	if (g_pSwapBuffersAddr)
	{
		DWORD dwOldProtect = 0;
		if (VirtualProtect(g_pSwapBuffersAddr, HOOK_SIZE,
			PAGE_EXECUTE_READWRITE, &dwOldProtect))
		{
			memcpy(g_pSwapBuffersAddr, g_SavedBytes, HOOK_SIZE);

			DWORD dwDummy = 0;
			VirtualProtect(g_pSwapBuffersAddr, HOOK_SIZE,
				dwOldProtect, &dwDummy);
		}
	}

	// Free trampoline.
	if (g_pTrampoline)
	{
		VirtualFree(g_pTrampoline, 0, MEM_RELEASE);
		g_pTrampoline = NULL;
	}

	g_pOrigSwapBuffers = NULL;
	g_pfnOnSwapBuffers = NULL;
	g_pSwapBuffersAddr = NULL;
	g_bInstalled = false;
	g_bFired = false;
}

void OpenGLHook::SetOnSwapBuffers(FnOnSwapBuffers pfn) { g_pfnOnSwapBuffers = pfn; }
BOOL OpenGLHook::HasFired() { return g_bFired ? TRUE : FALSE; }

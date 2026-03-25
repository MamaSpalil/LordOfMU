#ifndef __D3D9Hook_H
#define __D3D9Hook_H

#pragma once

#include <d3d9.h>

/**
 * \brief Hooks the Direct3D 9 device EndScene and Reset methods using vtable patching.
 *
 *        Workflow:
 *        1. Call Install() once after the game has initialized its D3D device.
 *           Install() creates a temporary D3D9 device to obtain the vtable,
 *           then patches EndScene (index 42) and Reset (index 16).
 *        2. Every frame the game calls EndScene -> our hook fires the
 *           registered callback so ImGui can render.
 *        3. On device Reset we invalidate / recreate ImGui device objects.
 *        4. Call Uninstall() on DLL detach to restore original pointers.
 *
 *        This approach works for both windowed and fullscreen Direct3D 9 games.
 */

// Callback signature for the overlay renderer (ImGui frame).
// pDevice is the game's IDirect3DDevice9.
typedef void (*FnOnEndScene)(IDirect3DDevice9* pDevice);
typedef void (*FnOnPreReset)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPP);
typedef void (*FnOnPostReset)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPP, HRESULT hr);

namespace D3D9Hook
{
	/// Install EndScene + Reset hooks.  Returns TRUE on success.
	/// \param hwndGame  The game window HWND (used to create a dummy device).
	BOOL Install(HWND hwndGame);

	/// Remove hooks and restore original vtable pointers.
	void Uninstall();

	/// Register / unregister callbacks.
	void SetOnEndScene(FnOnEndScene pfn);
	void SetOnPreReset(FnOnPreReset pfn);
	void SetOnPostReset(FnOnPostReset pfn);

	/// Returns the game's IDirect3DDevice9 captured on first EndScene call.
	/// May return NULL before the first frame.
	IDirect3DDevice9* GetDevice();
}

#endif // __D3D9Hook_H

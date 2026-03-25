#ifndef __OpenGLHook_H
#define __OpenGLHook_H

#pragma once

#include <windows.h>

/**
 * \brief Hooks the GDI32 SwapBuffers function using a 5-byte inline JMP.
 *
 *        Workflow:
 *        1. Call Install() once after the game's OpenGL context is ready.
 *           Install() locates SwapBuffers in gdi32.dll, creates a trampoline,
 *           and overwrites the first 5 bytes with a JMP to our hook.
 *        2. Every frame the game calls SwapBuffers -> our hook fires the
 *           registered callback so ImGui can render.
 *        3. Call Uninstall() on DLL detach to restore original bytes.
 *
 *        This approach intercepts ALL SwapBuffers calls in the process
 *        regardless of which module makes the call or which vtable is used,
 *        making it more reliable than vtable patching for OpenGL games.
 */

// Callback signature for the overlay renderer (ImGui frame).
// hdc is the device context passed to SwapBuffers.
typedef void (*FnOnSwapBuffers)(HDC hdc);

namespace OpenGLHook
{
	/// Install SwapBuffers hook.  Returns TRUE on success.
	BOOL Install();

	/// Remove hook and restore original bytes.
	void Uninstall();

	/// Register callback invoked just before every SwapBuffers call.
	void SetOnSwapBuffers(FnOnSwapBuffers pfn);

	/// Returns TRUE if the hook has successfully fired at least once.
	BOOL HasFired();
}

#endif // __OpenGLHook_H

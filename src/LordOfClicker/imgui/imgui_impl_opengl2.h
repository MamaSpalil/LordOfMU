// dear imgui: Renderer Backend for OpenGL2 (Legacy, Fixed-Function Pipeline)
// This needs to be used along with a Platform Backend (e.g. Win32)
//
// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' as ImTextureID. Read the FAQ about ImTextureID!
//
// Compatible with OpenGL 1.1+ (fixed-function pipeline).
// Suitable for MU Online and other legacy OpenGL games.

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
IMGUI_IMPL_API bool     ImGui_ImplOpenGL2_Init();
IMGUI_IMPL_API void     ImGui_ImplOpenGL2_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplOpenGL2_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplOpenGL2_RenderDrawData(ImDrawData* draw_data);

// Called by ImGui_ImplOpenGL2_NewFrame(), you may call this yourself as well.
IMGUI_IMPL_API bool     ImGui_ImplOpenGL2_CreateFontsTexture();
IMGUI_IMPL_API void     ImGui_ImplOpenGL2_DestroyFontsTexture();
IMGUI_IMPL_API bool     ImGui_ImplOpenGL2_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplOpenGL2_DestroyDeviceObjects();

#endif // #ifndef IMGUI_DISABLE

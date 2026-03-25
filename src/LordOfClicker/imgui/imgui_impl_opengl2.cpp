// dear imgui: Renderer Backend for OpenGL2 (Legacy, Fixed-Function Pipeline)
// This needs to be used along with a Platform Backend (e.g. Win32)
//
// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' as ImTextureID.
//
// Compatible with OpenGL 1.1+ (fixed-function pipeline).
// Uses vertex arrays (glVertexPointer, glTexCoordPointer, glColorPointer)
// and glDrawElements for rendering.

#ifdef _MSC_VER
#pragma warning(disable: 4505)  // unreferenced local function
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE
#include "imgui_impl_opengl2.h"

#include <windows.h>
#include <GL/gl.h>

// -----------------------------------------------------------------------
// Backend data stored in io.BackendRendererUserData
// -----------------------------------------------------------------------
struct ImGui_ImplOpenGL2_Data
{
    GLuint  FontTexture;
    ImGui_ImplOpenGL2_Data() : FontTexture(0) {}
};

static ImGui_ImplOpenGL2_Data* ImGui_ImplOpenGL2_GetBackendData()
{
    return ImGui::GetCurrentContext()
        ? (ImGui_ImplOpenGL2_Data*)ImGui::GetIO().BackendRendererUserData
        : NULL;
}

// -----------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------

bool ImGui_ImplOpenGL2_Init()
{
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    IM_ASSERT(io.BackendRendererUserData == NULL && "Already initialized!");

    ImGui_ImplOpenGL2_Data* bd = IM_NEW(ImGui_ImplOpenGL2_Data)();
    io.BackendRendererUserData = (void*)bd;
    io.BackendRendererName = "imgui_impl_opengl2";

    return true;
}

void ImGui_ImplOpenGL2_Shutdown()
{
    ImGui_ImplOpenGL2_Data* bd = ImGui_ImplOpenGL2_GetBackendData();
    IM_ASSERT(bd != NULL && "No renderer backend to shut down!");

    ImGui_ImplOpenGL2_DestroyDeviceObjects();
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = NULL;
    io.BackendRendererUserData = NULL;
    IM_DELETE(bd);
}

void ImGui_ImplOpenGL2_NewFrame()
{
    ImGui_ImplOpenGL2_Data* bd = ImGui_ImplOpenGL2_GetBackendData();
    IM_ASSERT(bd != NULL && "Context or backend not initialized!");

    if (!bd->FontTexture)
        ImGui_ImplOpenGL2_CreateDeviceObjects();
}

// -----------------------------------------------------------------------
// OpenGL2 Render function
// -----------------------------------------------------------------------

// Backup/restore helper to avoid affecting the game's GL state.
struct GLStateBackup
{
    GLint   LastTexture;
    GLint   LastPolygonMode[2];
    GLint   LastViewport[4];
    GLint   LastScissorBox[4];
    GLint   LastBlendSrc;
    GLint   LastBlendDst;
    GLenum  LastActiveTexture;
    GLboolean LastEnableBlend;
    GLboolean LastEnableCullFace;
    GLboolean LastEnableDepthTest;
    GLboolean LastEnableLighting;
    GLboolean LastEnableScissorTest;
    GLboolean LastEnableTexture2D;
    GLint   LastMatrixMode;
};

static void BackupGLState(GLStateBackup& s)
{
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &s.LastTexture);
    glGetIntegerv(GL_POLYGON_MODE, s.LastPolygonMode);
    glGetIntegerv(GL_VIEWPORT, s.LastViewport);
    glGetIntegerv(GL_SCISSOR_BOX, s.LastScissorBox);
    glGetIntegerv(GL_BLEND_SRC, &s.LastBlendSrc);
    glGetIntegerv(GL_BLEND_DST, &s.LastBlendDst);
    s.LastEnableBlend       = glIsEnabled(GL_BLEND);
    s.LastEnableCullFace    = glIsEnabled(GL_CULL_FACE);
    s.LastEnableDepthTest   = glIsEnabled(GL_DEPTH_TEST);
    s.LastEnableLighting    = glIsEnabled(GL_LIGHTING);
    s.LastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
    s.LastEnableTexture2D   = glIsEnabled(GL_TEXTURE_2D);
    glGetIntegerv(GL_MATRIX_MODE, &s.LastMatrixMode);
}

static void RestoreGLState(const GLStateBackup& s)
{
    glBindTexture(GL_TEXTURE_2D, s.LastTexture);
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum)s.LastPolygonMode[0]);
    glViewport(s.LastViewport[0], s.LastViewport[1],
        (GLsizei)s.LastViewport[2], (GLsizei)s.LastViewport[3]);
    glScissor(s.LastScissorBox[0], s.LastScissorBox[1],
        (GLsizei)s.LastScissorBox[2], (GLsizei)s.LastScissorBox[3]);
    glBlendFunc((GLenum)s.LastBlendSrc, (GLenum)s.LastBlendDst);
    if (s.LastEnableBlend)       glEnable(GL_BLEND);       else glDisable(GL_BLEND);
    if (s.LastEnableCullFace)    glEnable(GL_CULL_FACE);   else glDisable(GL_CULL_FACE);
    if (s.LastEnableDepthTest)   glEnable(GL_DEPTH_TEST);  else glDisable(GL_DEPTH_TEST);
    if (s.LastEnableLighting)    glEnable(GL_LIGHTING);    else glDisable(GL_LIGHTING);
    if (s.LastEnableScissorTest) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    if (s.LastEnableTexture2D)   glEnable(GL_TEXTURE_2D);  else glDisable(GL_TEXTURE_2D);
    glMatrixMode(s.LastMatrixMode);
}

void ImGui_ImplOpenGL2_RenderDrawData(ImDrawData* draw_data)
{
    // Avoid rendering when minimized.
    int fb_width  = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    // Backup GL state.
    GLStateBackup glstate;
    BackupGLState(glstate);

    // Setup render state: alpha-blending enabled, no face culling,
    // no depth testing, scissor enabled, vertex/texcoord/color arrays.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Setup viewport using draw_data->DisplaySize.
    glViewport(0, 0, fb_width, fb_height);

    // Setup orthographic projection matrix.
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // Use glOrtho with correct handedness for ImGui (Y increases downward).
    glOrtho((double)L, (double)R, (double)B, (double)T, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render command lists.
    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = draw_data->FramebufferScale;

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx*  idx_buffer = cmd_list->IdxBuffer.Data;

        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buffer + offsetof(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buffer + offsetof(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buffer + offsetof(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback)
            {
                if (pcmd->UserCallback != ImDrawCallback_ResetRenderState)
                    pcmd->UserCallback(cmd_list, pcmd);
                continue;
            }

            // Project scissor/clipping rectangles into framebuffer space.
            ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
            ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
            if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                continue;

            glScissor((int)clip_min.x,
                (int)((float)fb_height - clip_max.y),
                (int)(clip_max.x - clip_min.x),
                (int)(clip_max.y - clip_min.y));

            // Bind texture, draw elements.
            glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->GetTexID());

#ifdef IMGUI_USE_BGRA_PACKED_COLOR
            // Not used for OpenGL (OpenGL expects RGBA by default).
#endif

            glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                (const GLvoid*)(idx_buffer + pcmd->IdxOffset));
        }
    }

    // Restore modified state.
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    RestoreGLState(glstate);
}

// -----------------------------------------------------------------------
// Font texture
// -----------------------------------------------------------------------

bool ImGui_ImplOpenGL2_CreateFontsTexture()
{
    ImGui_ImplOpenGL2_Data* bd = ImGui_ImplOpenGL2_GetBackendData();
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Upload texture to GPU.
    GLint lastTex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);
    glGenTextures(1, &bd->FontTexture);
    glBindTexture(GL_TEXTURE_2D, bd->FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier.
    io.Fonts->SetTexID((ImTextureID)(intptr_t)bd->FontTexture);

    // Restore state.
    glBindTexture(GL_TEXTURE_2D, lastTex);

    return true;
}

void ImGui_ImplOpenGL2_DestroyFontsTexture()
{
    ImGui_ImplOpenGL2_Data* bd = ImGui_ImplOpenGL2_GetBackendData();
    if (bd->FontTexture)
    {
        ImGuiIO& io = ImGui::GetIO();
        glDeleteTextures(1, &bd->FontTexture);
        io.Fonts->SetTexID(0);
        bd->FontTexture = 0;
    }
}

bool ImGui_ImplOpenGL2_CreateDeviceObjects()
{
    return ImGui_ImplOpenGL2_CreateFontsTexture();
}

void ImGui_ImplOpenGL2_DestroyDeviceObjects()
{
    ImGui_ImplOpenGL2_DestroyFontsTexture();
}

#endif // #ifndef IMGUI_DISABLE

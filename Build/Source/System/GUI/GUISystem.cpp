#include "GUISystem.hpp"
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/win32/imgui_impl_win32.h"
#include "../../External/imgui/dx11/imgui_impl_dx11.h"
#include "../Core/OS-API/Application.hpp"
#include "../Core/OS-API/OSCommon.hpp"
#include "../Graphics/RenderSystem.hpp"
#include "../Graphics/Renderer/RendererCommon.hpp"
#include "Editor/GameEditor.hpp"

namespace Engine5
{
    GUISystem::GUISystem(ImGuiIO& io)
        : m_im_gui_io(io)
    {
    }

    GUISystem::~GUISystem()
    {
    }

    void GUISystem::Initialize(Application* application)
    {
        (void)m_im_gui_io;
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
        ImGuiStyle& style    = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
#if defined(E5_WIN32)
        ImGui_ImplWin32_Init(application->GetOperatingSystem()->AppHWnd());
#endif
#if defined(E5_DIRECTX11)
        m_renderer = application->GetRenderSystem()->GetRenderer();
        ImGui_ImplDX11_Init(m_renderer->GetDevice(), m_renderer->GetDeviceContext());
#endif
    }

    void GUISystem::Shutdown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void GUISystem::BeginUpdate() const
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void GUISystem::EndUpdate()
    {
        m_game_editor->Update();
        //// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window)
        //    ImGui::ShowDemoWindow(&show_demo_window);
        //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        //{
        //    static float f       = 0.0f;
        //    static int   counter = 0;
        //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bool storing our window open/close state
        //    ImGui::Checkbox("Another Window", &show_another_window);
        //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //        counter++;
        //    ImGui::SameLine();
        //    ImGui::Text("counter = %d", counter);
        //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //    ImGui::End();
        //}
        //// 3. Show another simple window.
        //if (show_another_window)
        //{
        //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //    ImGui::Text("Hello from another window!");
        //    if (ImGui::Button("Close Me"))
        //        show_another_window = false;
        //    ImGui::End();
        //}
    }

    void GUISystem::BeginRender() const
    {
        ImGui::Render();
        m_renderer->SetBackBufferRenderTarget();
    }

    void GUISystem::EndRender() const
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    bool GUISystem::IsFocusGUI()
    {
        return ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
    }

    void GUISystem::CreateGUIContext()
    {
        // Setup Dear ImGui context
#if defined(E5_WIN32)
        ImGui_ImplWin32_EnableDpiAwareness();
#endif
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }

    void GUISystem::GenerateGUIKeyMap()
    {
    }

    void GUISystem::AddGUI(GameEditor* game_editor)
    {
        m_game_editor = game_editor;
    }
}

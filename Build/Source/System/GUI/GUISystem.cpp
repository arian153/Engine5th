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
        //m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI
        // Setup Dear ImGui style
        ImGui::StyleColorsLight();
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

    void GUISystem::Update(Real dt)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        m_game_editor->Update(dt);
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
        if (m_b_free_focus)
            return false;
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

    void GUISystem::SetFocusFree(bool flag)
    {
        m_b_free_focus = flag;
    }

    void GUISystem::AddGUI(GameEditor* game_editor)
    {
        m_game_editor = game_editor;
    }

    bool GUISystem::OnGameEditor() const
    {
        return m_game_editor->IsOpen();
    }
}

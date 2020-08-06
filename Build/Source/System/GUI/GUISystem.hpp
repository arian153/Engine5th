#pragma once
#include "../../External/imgui/imgui.h"

namespace Engine5
{
    class RendererCommon;
    class Application;

    class GUISystem
    {
    public:
        GUISystem();
        ~GUISystem();

        void Initialize(Application* application);
        void Shutdown();

        void BeginUpdate() const;
        void EndUpdate();

        void BeginRender() const;
        void EndRender() const;

        void OnResize(int width, int height);
        void OnFullscreen() const;

    private:
        ImGuiIO         m_fake_io;
        ImGuiIO&        m_im_gui_io = m_fake_io;
        RendererCommon* m_renderer  = nullptr;

        bool show_demo_window    = true;
        bool show_another_window = false;
    };
}

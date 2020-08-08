#pragma once
#include "../../External/imgui/imgui.h"

namespace Engine5
{
    class RendererCommon;
    class Application;

    class GUISystem
    {
    public:
        explicit GUISystem(ImGuiIO& io);
        ~GUISystem();

        void Initialize(Application* application);
        void Shutdown();

        void BeginUpdate() const;
        void EndUpdate();

        void BeginRender() const;
        void EndRender() const;

        bool IsFocusGUI() const;

        static void CreateGUIContext();

    private:
        ImGuiIO&        m_im_gui_io;
        RendererCommon* m_renderer = nullptr;

        bool show_demo_window    = true;
        bool show_another_window = false;
    };
}

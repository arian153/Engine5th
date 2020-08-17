#pragma once
#include "../../External/imgui/imgui.h"
#include "../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class GameEditor;
    class RendererCommon;
    class Application;

    class GUISystem
    {
    public:
        explicit GUISystem(ImGuiIO& io);
        ~GUISystem();

        void Initialize(Application* application);
        void Shutdown();

        void Update(Real dt);

        void BeginRender() const;
        void EndRender() const;

        static bool IsFocusGUI();
        static void CreateGUIContext();

        void AddGUI(GameEditor* game_editor);
        bool OnGameEditor() const;

    private:
        ImGuiIO&        m_im_gui_io;
        RendererCommon* m_renderer    = nullptr;
        GameEditor*     m_game_editor = nullptr;
    };
}

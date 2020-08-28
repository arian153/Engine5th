#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "LevelEditor.hpp"
#include "SpaceEditor.hpp"
#include "../../../External/imgui/imgui.h"
#include "Command/CommandRegistry.hpp"

namespace Engine5
{
    class GUISystem;
    class Application;

    class GameEditor
    {
    public:
        GameEditor();
        ~GameEditor();

        void Initialize(Application* application);
        void Update(Real dt);
        void Shutdown();

        bool IsOpen() const;

    private:
        void UpdateFileTab();
        void UpdateEditTab();
        void UpdateObjectTab();

    private:
        LevelEditor     m_level_editor;
        SpaceEditor     m_space_editor;
        Application*    m_application = nullptr;
        CommandRegistry m_command_registry;

        //editor flag
        bool        m_b_open       = true;
        std::string m_editor_label = "";

        //imgui flag
        ImGuiDockNodeFlags m_dock_space_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags   m_window_flags     = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    };
}

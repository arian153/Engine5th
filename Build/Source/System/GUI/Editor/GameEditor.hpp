#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "LevelEditor.hpp"
#include "SpaceEditor.hpp"
#include "../../../External/imgui/imgui.h"

namespace Engine5
{
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
        LevelEditor m_level_editor;
        SpaceEditor m_space_editor;
        Application* m_application = nullptr;

        //editor flag
        bool        m_b_open    = true;
        std::string m_editor_label = "";

        //imgui flag
        ImGuiDockNodeFlags m_dock_space_flags = ImGuiDockNodeFlags_None;
    };
}

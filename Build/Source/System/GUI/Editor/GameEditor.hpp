#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "LevelEditor.hpp"
#include "SpaceEditor.hpp"
#include "../../../External/imgui/imgui.h"

namespace Engine5
{
    class GameEditor
    {
    public:
        GameEditor();
        ~GameEditor();

        void Initialize();
        void Update();
        void Shutdown();

    private:
        LevelEditor m_level_editor;
        SpaceEditor m_space_editor;

    private:
        bool               m_b_open_editor    = true;
        ImGuiDockNodeFlags m_dock_space_flags = ImGuiDockNodeFlags_None;
    };
}

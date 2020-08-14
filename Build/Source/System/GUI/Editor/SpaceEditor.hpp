#pragma once
#include "../../../External/imgui/imgui.h"
#include <vector>
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class ResourceManager;
    class SpaceManager;
    class Application;
    class RenderTextureGenerator;
    class Space;

    class SpaceEditor
    {
    public:
        SpaceEditor();
        ~SpaceEditor();

        void Initialize(Application* application);
        void Update();

    private:
        void UpdateMenu();
        void UpdateTab();
        void CloseTab();

    private:
        void DoOpen(Space* space);
        void DoQueueClose(Space* space);
        void DoForceClose(Space* space);
        void DoSave(Space* space);
        void DisplayContents(Space* space);
        void DisplayContextMenu(Space* space);
        void DisplayScene(Space* space) const;

    private:
        friend class GameEditor;

    private:
        bool             m_b_open        = false;
        bool             m_b_reorderable = true;
        ImGuiTabBarFlags m_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;
        ImVec2           m_uv_min        = ImVec2(0.0f, 0.0f);                   // Top-left
        ImVec2           m_uv_max        = ImVec2(1.0f, 1.0f);                   // Lower-right
        ImVec4           m_tint_col      = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
        ImVec4           m_border_col    = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        Real             m_scene_scale   = 1024.0f;

    private:
        //Space*           m_editing_space    = nullptr;
        GameEditor*      m_game_editor      = nullptr;
        Application*     m_application      = nullptr;
        SpaceManager*    m_space_manager    = nullptr;
        ResourceManager* m_resource_manager = nullptr;
        //tool
        std::vector<Space*>*    m_spaces = nullptr;
        std::vector<Space*>     m_close_queue;
        RenderTextureGenerator* m_render_texture_generator = nullptr;
    };
}

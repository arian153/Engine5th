#pragma once
#include "../../../External/imgui/imgui.h"
#include <vector>

namespace Engine5
{
    class SpaceManager;
    class Application;
    class RenderTextureGenerator;
    class Space;

    class SpaceEditor
    {
    public:
        SpaceEditor();
        ~SpaceEditor();

        bool AddSpace(Space* space);

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

    private:
        friend class GameEditor;

    private:
        bool             m_b_open        = false;
        bool             m_b_reorderable = true;
        ImGuiTabBarFlags m_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;

    private:
        Space*        m_editing_space = nullptr;
        GameEditor*   m_game_editor   = nullptr;
        Application*  m_application   = nullptr;
        SpaceManager* m_space_manager = nullptr;
        //tool
        std::vector<Space*>     m_spaces;
        std::vector<Space*>     m_close_queue;
        RenderTextureGenerator* m_render_texture_generator = nullptr;
    };
}

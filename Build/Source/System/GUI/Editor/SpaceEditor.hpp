#pragma once
#include "../../../External/imgui/imgui.h"

namespace Engine5
{
    class Application;
    class RenderTextureGenerator;
    class Space;

    class SpaceEditor
    {
    public:
        SpaceEditor();
        ~SpaceEditor();

        bool Open(Space* space);
        void Close();

        void Update();

    private:
        friend class GameEditor;

    private:
        bool   m_b_open        = false;
        Space* m_editing_space = nullptr;

    private:
        GameEditor*  m_game_editor = nullptr;
        Application* m_application = nullptr;
        //tool
        RenderTextureGenerator* m_render_to_texture_generator = nullptr;
    };
}

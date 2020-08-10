#pragma once
#include "../../../External/imgui/imgui.h"

namespace Engine5
{
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
    };
}

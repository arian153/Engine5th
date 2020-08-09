#pragma once

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




    private:
        Space* m_editing_space = nullptr;
    };
}

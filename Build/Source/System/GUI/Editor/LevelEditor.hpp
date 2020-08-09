#pragma once

namespace Engine5
{
    class Level;

    class LevelEditor
    {
    public:
        LevelEditor();
        ~LevelEditor();

        bool Open(Level* space);
        void Close();

    private:
        Level* m_editing_level = nullptr;
    };
}

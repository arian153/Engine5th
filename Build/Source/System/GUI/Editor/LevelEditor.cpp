#include "LevelEditor.hpp"

namespace Engine5
{
    LevelEditor::LevelEditor()
    {
    }

    LevelEditor::~LevelEditor()
    {
    }

    bool LevelEditor::Open(Level* level)
    {
        if (level != nullptr)
        {
            m_editing_level = level;
            return true;
        }
        return false;
    }

    void LevelEditor::Close()
    {
        m_editing_level = nullptr;
    }
}

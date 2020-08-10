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

    void LevelEditor::Update()
    {
        if (m_b_open)
        {
            ImGui::Begin("Level Editor", &m_b_open);
            ImGui::Text("Level Editor");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
    }
}

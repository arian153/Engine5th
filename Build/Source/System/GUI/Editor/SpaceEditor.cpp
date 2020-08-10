#include "SpaceEditor.hpp"

namespace Engine5
{
    SpaceEditor::SpaceEditor()
    {
    }

    SpaceEditor::~SpaceEditor()
    {
    }

    bool SpaceEditor::Open(Space* space)
    {
        if (space != nullptr)
        {
            m_editing_space = space;
            return true;
        }
        return false;
    }

    void SpaceEditor::Close()
    {
        m_editing_space = nullptr;
    }

    void SpaceEditor::Update()
    {
        if (m_b_open)
        {
            ImGui::Begin("Space Editor", &m_b_open);
            ImGui::Text("Space Editor");
            ImGui::End();
        }
    }
}

#include "SpaceEditor.hpp"
#include "GameEditor.hpp"
#include "../../Core/OS-API/Application.hpp"
#include "../../../Manager/Resource/ResourceManager.hpp"
#include "../../../Manager/Resource/ResourceType/TextureResource.hpp"
#include "../../Graphics/Texture/TextureCommon.hpp"

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

            ImVec2 uv_min     = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max     = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_col   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            ImGui::Image(m_application->GetResourceManager()->GetTextureResource(L"../../Resource/Texture/doodle01.png")->GetTexture()->GetTexture(), 
                ImVec2(512, 512), uv_min, uv_max, tint_col, border_col);
            ImGui::End();
        }
    }
}

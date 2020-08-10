#include "GameEditor.hpp"
#include "../../../External/imgui/imgui.h"

namespace Engine5
{
    GameEditor::GameEditor()
    {
    }

    GameEditor::~GameEditor()
    {
    }

    void GameEditor::Initialize()
    {
    }

    void GameEditor::Update()
    {
        if (m_b_open_editor)
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            ImGuiViewport*   viewport     = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            if (m_dock_space_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Game Editor", &m_b_open_editor, window_flags);
            ImGui::PopStyleVar(3);
            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dock_space_id = ImGui::GetID("Game Editor");
                ImGui::DockSpace(dock_space_id, ImVec2(0.0f, 0.0f), m_dock_space_flags);
            }
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Game Editor"))
                {
                    if (ImGui::MenuItem("Pass through editor window", "", (m_dock_space_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
                        m_dock_space_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close Game Editor", nullptr, false))
                        m_b_open_editor = false;
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }
    }

    void GameEditor::Shutdown()
    {
    }
}

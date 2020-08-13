#include "GameEditor.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../Core/OS-API/Application.hpp"
#include "../../../Manager/Level/LevelManager.hpp"

namespace Engine5
{
    GameEditor::GameEditor()
    {
    }

    GameEditor::~GameEditor()
    {
    }

    void GameEditor::Initialize(Application* application)
    {
        m_application = application;
        m_space_editor.m_game_editor = this;
        m_space_editor.Initialize(application);
    }

    void GameEditor::Update()
    {
        if (m_b_open)
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
            ImGui::Begin("Game Editor", &m_b_open, window_flags);
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
                    m_editor_label = m_level_editor.m_b_open ? "Close Level Editor" : "Open Level Editor";
                    if (ImGui::MenuItem(m_editor_label.c_str(), ""))
                        m_level_editor.m_b_open = !m_level_editor.m_b_open;
                    m_editor_label = m_space_editor.m_b_open ? "Close Space Editor" : "Open Space Editor";
                    if (ImGui::MenuItem(m_editor_label.c_str(), ""))
                        m_space_editor.m_b_open = !m_space_editor.m_b_open;
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close Game Editor", "Tab+Space"))
                    {
                        m_b_open = false;
                    }
                    if (ImGui::MenuItem("Close Application", nullptr, false))
                        m_application->GetLevelManager()->SetQuit();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();
            {
                m_level_editor.Update();
                m_space_editor.Update();
            }
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) && ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Tab)))
            {
                m_b_open = false;
            }
        }
        else
        {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) && ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Tab)))
            {
                m_b_open = true;
            }
        }
    }

    void GameEditor::Shutdown()
    {
    }
}

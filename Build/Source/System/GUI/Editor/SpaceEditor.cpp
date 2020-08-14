#include "SpaceEditor.hpp"
#include "GameEditor.hpp"
#include "../../Core/OS-API/Application.hpp"
#include "../../../Manager/Resource/ResourceManager.hpp"
#include "../../../Manager/Resource/ResourceType/TextureResource.hpp"
#include "../../Graphics/Texture/TextureCommon.hpp"
#include "../../Graphics/RenderSystem.hpp"
#include "../../Graphics/Utility/RenderTextureGenerator.hpp"
#include "../../../Manager/Space/Space.hpp"
#include "../../../Manager/Level/LevelManager.hpp"
#include "../../../Manager/Level/Level.hpp"
#include "../../../Manager/Space/SpaceManager.hpp"

namespace Engine5
{
    SpaceEditor::SpaceEditor()
    {
    }

    SpaceEditor::~SpaceEditor()
    {
    }

    void SpaceEditor::Initialize(Application* application)
    {
        m_application              = application;
        m_resource_manager         = application->GetResourceManager();
        m_render_texture_generator = application->GetRenderSystem()->GetRenderTextureGenerator();
        m_space_manager            = application->GetSpaceManager();
        m_spaces                   = &m_space_manager->m_spaces;
    }

    void SpaceEditor::Update()
    {
        if (m_b_open)
        {
            ImGui::Begin("Space Editor", &m_b_open, ImGuiWindowFlags_MenuBar);
            UpdateMenu();
            ImGui::Text("Space Editor");
            UpdateTab();
            CloseTab();
            ImGui::End();
        }
    }

    void SpaceEditor::UpdateMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                int open_count = 0;
                for (size_t i = 0; i < m_spaces->size(); ++i)
                    open_count += m_spaces->at(i)->m_b_curr_open ? 1 : 0;
                if (ImGui::BeginMenu("Open", open_count < m_spaces->size()))
                {
                    for (size_t i = 0; i < m_spaces->size(); ++i)
                    {
                        Space* space = m_spaces->at(i);
                        if (!space->m_b_curr_open)
                            if (ImGui::MenuItem(space->m_name.c_str()))
                            {
                                DoOpen(space);
                            }
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Close All Documents", nullptr, false, open_count > 0))
                    for (size_t i = 0; i < m_spaces->size(); ++i)
                        DoQueueClose(m_spaces->at(i));
                if (ImGui::MenuItem("Exit", "Alt+F4"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void SpaceEditor::UpdateTab()
    {
        ImGuiTabBarFlags tab_bar_flags = (m_fitting_flags) | (m_b_reorderable ? ImGuiTabBarFlags_Reorderable : 0);
        if (ImGui::BeginTabBar("##tabs", tab_bar_flags))
        {
            if (m_b_reorderable)
            {
                for (size_t i = 0; i < m_spaces->size(); ++i)
                {
                    Space* space = m_spaces->at(i);
                    if (!space->m_b_curr_open && space->m_b_prev_open)
                    {
                        ImGui::SetTabItemClosed(space->m_name.c_str());
                    }
                    space->m_b_prev_open = space->m_b_curr_open;
                }
            }
            // [DEBUG] Stress tests
            //if ((ImGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
            //if (ImGui::GetIO().KeyCtrl) ImGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..
            // Submit Tabs
            for (size_t i = 0; i < m_spaces->size(); ++i)
            {
                Space* space = m_spaces->at(i);
                if (!space->m_b_curr_open)
                {
                    continue;
                }
                ImGuiTabItemFlags tab_flags = (space->m_b_modified ? ImGuiTabItemFlags_UnsavedDocument : 0);
                bool              visible   = ImGui::BeginTabItem(space->m_name.c_str(), &space->m_b_curr_open, tab_flags);
                // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                if (!space->m_b_curr_open && space->m_b_modified)
                {
                    space->m_b_curr_open = true;
                    DoQueueClose(space);
                }
                DisplayContextMenu(space);
                if (visible)
                {
                    DisplayContents(space);
                    DisplayScene(space);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }

    void SpaceEditor::CloseTab()
    {
        if (m_close_queue.empty())
        {
            // Close queue is locked once we started a popup
            for (size_t i = 0; i < m_spaces->size(); ++i)
            {
                Space* space = m_spaces->at(i);
                if (space->m_b_close)
                {
                    space->m_b_close = false;
                    m_close_queue.push_back(space);
                }
            }
        }
        if (!m_close_queue.empty())
        {
            int close_queue_unsaved_documents = 0;
            for (size_t i = 0; i < m_close_queue.size(); ++i)
                if (m_close_queue[i]->m_b_modified)
                    close_queue_unsaved_documents++;
            if (close_queue_unsaved_documents == 0)
            {
                // Close documents when all are unsaved
                for (size_t i = 0; i < m_close_queue.size(); ++i)
                    DoForceClose(m_close_queue[i]);
                m_close_queue.clear();
            }
            else
            {
                if (!ImGui::IsPopupOpen("Save?"))
                    ImGui::OpenPopup("Save?");
                if (ImGui::BeginPopupModal("Save?"))
                {
                    ImGui::Text("Save change to the following items?");
                    ImGui::SetNextItemWidth(-1.0f);
                    if (ImGui::ListBoxHeader("##", close_queue_unsaved_documents, 6))
                    {
                        for (size_t i = 0; i < m_close_queue.size(); ++i)
                            if (m_close_queue[i]->m_b_modified)
                                ImGui::Text("%s", m_close_queue[i]->m_name.c_str());
                        ImGui::ListBoxFooter();
                    }
                    if (ImGui::Button("Yes", ImVec2(80, 0)))
                    {
                        for (size_t i = 0; i < m_close_queue.size(); ++i)
                        {
                            if (m_close_queue[i]->m_b_modified)
                                DoSave(m_close_queue[i]);
                            DoForceClose(m_close_queue[i]);
                        }
                        m_close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("No", ImVec2(80, 0)))
                    {
                        for (size_t i = 0; i < m_close_queue.size(); ++i)
                            DoForceClose(m_close_queue[i]);
                        m_close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(80, 0)))
                    {
                        m_close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
        }
    }

    void SpaceEditor::DoOpen(Space* space)
    {
        space->m_b_curr_open = true;
    }

    void SpaceEditor::DoQueueClose(Space* space)
    {
        space->m_b_close = true;
    }

    void SpaceEditor::DoForceClose(Space* space)
    {
        space->m_b_curr_open = false;
        space->m_b_modified  = false;
    }

    void SpaceEditor::DoSave(Space* space)
    {
        space->m_b_modified = false;
    }

    void SpaceEditor::DisplayContents(Space* space)
    {
        ImGui::PushID(space);
        ImGui::Text(space->m_name.c_str());
        //ImGui::PushStyleColor(ImGuiCol_Text, doc->Color);
        ImGui::TextWrapped("Hello World.");
        //ImGui::PopStyleColor();
        if (ImGui::Button("Modify", ImVec2(100, 0)))
            space->m_b_modified = true;
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(100, 0)))
            DoSave(space);
        ImGui::PopID();
    }

    void SpaceEditor::DisplayContextMenu(Space* space)
    {
        if (!ImGui::BeginPopupContextItem())
            return;
        if (ImGui::MenuItem(("Save " + space->m_name).c_str(), "CTRL+S", false, space->m_b_curr_open))
            DoSave(space);
        if (ImGui::MenuItem("Close", "CTRL+W", false, space->m_b_curr_open))
            DoQueueClose(space);
        ImGui::EndPopup();
    }

    void SpaceEditor::DisplayScene(Space* space) const
    {
        if (space != nullptr)
        {
            m_render_texture_generator->BeginRenderToTexture(ColorDef::Pure::Black);
            m_render_texture_generator->Render(space->GetScene());
            m_render_texture_generator->EndRenderToTexture();
            ImGui::Image(
                         m_render_texture_generator->GetTexture()->GetTexture(),
                         ImVec2(512, 512), m_uv_min, m_uv_max, m_tint_col, m_border_col);
        }
    }
}

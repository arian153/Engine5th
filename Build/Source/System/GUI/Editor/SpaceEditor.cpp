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
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../../Manager/Resource/ResourceType/JsonResource.hpp"

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
                for (size_t i = 0; i < m_space_resources.size(); ++i)
                    open_count += m_space_resources.at(i)->IsOpen() ? 1 : 0;
                if (ImGui::BeginMenu("Open", open_count < m_space_resources.size()))
                {
                    for (size_t i = 0; i < m_space_resources.size(); ++i)
                    {
                        JsonResource* resource = m_space_resources.at(i);
                        if (!resource->IsOpen())
                            if (ImGui::MenuItem(resource->FileName().c_str()))
                            {
                                DoOpen(resource);
                            }
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Close All Documents", nullptr, false, open_count > 0))
                    for (size_t i = 0; i < m_space_resources.size(); ++i)
                        DoQueueClose(m_space_resources.at(i));
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
                for (size_t i = 0; i < m_space_resources.size(); ++i)
                {
                    JsonResource* resource = m_space_resources.at(i);
                    if (!resource->IsOpen() && resource->WasOpen())
                    {
                        ImGui::SetTabItemClosed(resource->FileName().c_str());
                    }
                    resource->UpdateOpenState();
                }
            }
            // [DEBUG] Stress tests
            //if ((ImGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
            //if (ImGui::GetIO().KeyCtrl) ImGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..
            // Submit Tabs
            for (size_t i = 0; i < m_space_resources.size(); ++i)
            {
                JsonResource* resource = m_space_resources.at(i);
                if (!resource->IsOpen())
                {
                    continue;
                }
                ImGuiTabItemFlags tab_flags = (resource->IsModified() ? ImGuiTabItemFlags_UnsavedDocument : 0);
                bool              visible   = ImGui::BeginTabItem(resource->FileName().c_str(), &resource->IsOpen(), tab_flags);
                // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                if (!resource->IsOpen() && resource->IsModified())
                {
                    resource->IsOpen() = true;
                    DoQueueClose(resource);
                }
                DisplayContextMenu(resource);
                if (visible)
                {
                    DisplayContents(resource);
                    DisplayScene();
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
            for (size_t i = 0; i < m_space_resources.size(); ++i)
            {
                JsonResource* space = m_space_resources.at(i);
                if (space->IsClose())
                {
                    space->IsClose() = false;
                    m_close_queue.push_back(space);
                }
            }
        }
        if (!m_close_queue.empty())
        {
            int close_queue_unsaved_documents = 0;
            for (size_t i = 0; i < m_close_queue.size(); ++i)
                if (m_close_queue[i]->IsModified())
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
                            if (m_close_queue[i]->IsModified())
                                ImGui::Text("%s", m_close_queue[i]->FileName().c_str());
                        ImGui::ListBoxFooter();
                    }
                    if (ImGui::Button("Yes", ImVec2(80, 0)))
                    {
                        for (size_t i = 0; i < m_close_queue.size(); ++i)
                        {
                            if (m_close_queue[i]->IsModified())
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

    void SpaceEditor::DoOpen(JsonResource* resource)
    {
        resource->IsOpen() = true;
    }

    void SpaceEditor::DoQueueClose(JsonResource* resource)
    {
        resource->IsClose() = true;
    }

    void SpaceEditor::DoForceClose(JsonResource* resource)
    {
        resource->IsOpen()     = false;
        resource->IsModified() = false;
    }

    void SpaceEditor::DoSave(JsonResource* resource)
    {
        resource->IsModified() = false;
    }

    void SpaceEditor::DisplayContents(JsonResource* resource)
    {
        ImGui::PushID(resource);
        ImGui::Text(resource->FileName().c_str());
        if (ImGui::Button("Modify", ImVec2(100, 0)))
            resource->IsModified() = true;
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(100, 0)))
            DoSave(resource);
        ImGui::PopID();
    }

    void SpaceEditor::DisplayContextMenu(JsonResource* resource)
    {
        if (!ImGui::BeginPopupContextItem())
            return;
        if (ImGui::MenuItem(("Save " + resource->FileName()).c_str(), "CTRL+S", false, resource->IsOpen()))
            DoSave(resource);
        if (ImGui::MenuItem("Close", "CTRL+W", false, resource->IsOpen()))
            DoQueueClose(resource);
        ImGui::EndPopup();
    }

    void SpaceEditor::DisplayScene() const
    {
        if (m_editing_space != nullptr)
        {
            auto scene = m_editing_space->GetScene();
            Real ratio = 1.0f;
            m_render_texture_generator->BeginRenderToTexture(ColorDef::Pure::Gray);
            if (scene != nullptr)
            {
                m_render_texture_generator->Render(scene);
                ratio = scene->GetAspectRatio();
            }
            m_render_texture_generator->EndRenderToTexture();
            ImGui::Image(
                         m_render_texture_generator->GetTexture()->GetTexture(),
                         ImVec2(m_scene_scale * ratio, m_scene_scale), m_uv_min, m_uv_max, m_tint_col, m_border_col);
        }
    }
}

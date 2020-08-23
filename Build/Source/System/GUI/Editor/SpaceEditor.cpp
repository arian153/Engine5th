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
#include "../GUISystem.hpp"
#include "../../../Manager/Object/ObjectManager.hpp"
#include "../../../Manager/Object/Object.hpp"
#include "../../../Manager/Component/Component.hpp"

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
        m_resource_manager->GetJsonResources(eJsonType::Space, m_resources);
    }

    void SpaceEditor::SetStepFrameTime(Real target_frame_per_second)
    {
        m_time_step = Math::IsZero(target_frame_per_second) ? 0.0f : 1.0f / target_frame_per_second;
    }

    void SpaceEditor::UpdateSceneWindow(Real dt)
    {
        m_editing_space = nullptr;
        ImGui::Begin("Space");
        ImGuiTabBarFlags tab_bar_flags = (m_fitting_flags) | (m_b_reorderable ? ImGuiTabBarFlags_Reorderable : 0);
        if (ImGui::BeginTabBar("##tabs", tab_bar_flags))
        {
            if (m_b_reorderable)
            {
                for (size_t i = 0; i < m_resources.size(); ++i)
                {
                    JsonResource* resource = m_resources.at(i);
                    if (!resource->IsOpen() && resource->WasOpen())
                    {
                        ImGui::SetTabItemClosed(resource->FileName().c_str());
                    }
                    resource->UpdateOpenState();
                }
            }
            // Submit Tabs
            for (size_t i = 0; i < m_resources.size(); ++i)
            {
                JsonResource* resource = m_resources.at(i);
                if (!resource->IsOpen())
                {
                    continue;
                }
                ImGuiTabItemFlags tab_flags = (resource->IsModified() ? ImGuiTabItemFlags_UnsavedDocument : 0);
                bool              visible   = ImGui::BeginTabItem(resource->FileName().c_str(), &resource->IsOpen(), tab_flags);
                // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                if (!resource->IsOpen())
                {
                    if (resource->IsModified())
                    {
                        resource->IsOpen() = true;
                    }
                    DoQueueClose(resource);
                }
                DisplayContextMenu(resource);
                if (visible)
                {
                    m_space_index = i;
                    DisplayContents(resource);
                    m_editing_space = DisplayScene(resource->FileName(), dt);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
        {
            if (m_close_queue.empty())
            {
                // Close queue is locked once we started a popup
                for (size_t i = 0; i < m_resources.size(); ++i)
                {
                    JsonResource* resource = m_resources.at(i);
                    if (resource->IsClose())
                    {
                        resource->IsClose() = false;
                        m_close_queue.push_back(resource);
                        m_editing_space = nullptr;
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
    }

    void SpaceEditor::UpdateInspectorWindow()
    {
        ImGui::Begin("Inspector");
        if (m_editing_object != nullptr)
        {
            for (auto& component : m_editing_object->m_components)
            {
                component.second->Edit();
            }
        }
        ImGui::End();
    }

    static bool Items_ObjectGetter(void* data, int idx, const char** out_text)
    {
        auto& objects = *static_cast<std::vector<Object*>*>(data);
        if (idx < 0 || idx >= static_cast<int>(objects.size()))
        {
            return false;
        }
        *out_text = objects[idx]->GetNameChar();
        return true;
    }

    void SpaceEditor::UpdateHierarchyWindow()
    {
        ImGui::Begin("Hierarchy");
        if (m_editing_space != nullptr)
        {
            auto objects = m_editing_space->GetObjectManager()->m_objects;
            if (ImGui::ListBox(
                               "##list", &m_object_index, Items_ObjectGetter,
                               static_cast<void*>(&objects), static_cast<int>(objects.size()), 20))
            {
                m_editing_object = objects[m_object_index];
            }
        }
        ImGui::End();
    }

    void SpaceEditor::OpenSequence()
    {
        for (size_t i = 0; i < m_resources.size(); ++i)
        {
            JsonResource* resource = m_resources.at(i);
            if (!resource->IsOpen())
                if (ImGui::MenuItem(resource->FileName().c_str()))
                {
                    DoOpen(resource);
                }
        }
    }

    void SpaceEditor::CloseAllSequence()
    {
        for (size_t i = 0; i < m_resources.size(); ++i)
        {
            DoQueueClose(m_resources.at(i));
        }
    }

    void SpaceEditor::CloseSequence()
    {
        DoQueueClose(m_resources.at(m_space_index));
    }

    size_t SpaceEditor::OpenCount() const
    {
        size_t open_count = 0;
        for (size_t i = 0; i < m_resources.size(); ++i)
            open_count += m_resources.at(i)->IsOpen() ? 1 : 0;
        return open_count;
    }

    size_t SpaceEditor::Size() const
    {
        return m_resources.size();
    }

    void SpaceEditor::DoOpen(JsonResource* resource)
    {
        resource->IsOpen() = true;
        auto found         = m_editing_spaces.find(resource->FileName());
        if (found == m_editing_spaces.end())
        {
            m_editing_spaces.emplace(resource->FileName(), m_space_manager->CreateSpace(resource));
        }
    }

    void SpaceEditor::DoQueueClose(JsonResource* resource)
    {
        resource->IsClose() = true;
        m_editing_space     = nullptr;
    }

    void SpaceEditor::DoForceClose(JsonResource* resource)
    {
        resource->IsOpen()     = false;
        resource->IsModified() = false;
        auto found             = m_editing_spaces.find(resource->FileName());
        if (found != m_editing_spaces.end())
        {
            m_space_manager->RemoveSpace(found->second);
            m_editing_spaces.erase(found);
        }
    }

    void SpaceEditor::DoSave(JsonResource* resource)
    {
        resource->IsModified() = false;
    }

    void SpaceEditor::DisplayContents(JsonResource* resource)
    {
        ImGui::PushID(resource);
        std::string label = m_b_pause ? "Resume" : "Pause";
        if (ImGui::Button(label.c_str(), ImVec2(100, 0)))
        {
            m_b_pause = !m_b_pause;
        }
        if (m_b_pause)
        {
            ImGui::SameLine();
            if (ImGui::Button("Update", ImVec2(100, 0)))
            {
                m_b_step = true;
            }
            m_b_step = ImGui::IsItemActive();
            ImGui::SameLine();
            if (ImGui::Button("Step", ImVec2(100, 0)))
            {
                m_b_step = true;
            }
        }
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

    Space* SpaceEditor::DisplayScene(const std::string& name, Real dt) const
    {
        auto found = m_editing_spaces.find(name);
        auto space = found->second;
        if (space != nullptr)
        {
            if (m_b_pause && m_b_step || !m_b_pause)
            {
                space->Update(dt);
            }
            else
            {
            }
            m_render_texture_generator->BeginRenderToTexture(ColorDef::Pure::Gray);
            m_render_texture_generator->Render(space);
            m_render_texture_generator->EndRenderToTexture();
            ImVec2 min         = ImGui::GetWindowContentRegionMin();
            ImVec2 max         = ImGui::GetWindowContentRegionMax();
            Real   scene_scale = max.x - min.x;
            auto   scene       = space->GetScene();
            Real   ratio       = scene != nullptr ? 1.0f / scene->GetAspectRatio() : 1.0f;
            ImGui::Image(
                         m_render_texture_generator->GetTexture()->GetTexture(),
                         ImVec2(scene_scale, scene_scale * ratio), m_uv_min, m_uv_max, m_tint_col, m_border_col);
            if (ImGui::IsItemHovered())
            {
                GUISystem::SetFocusFree(true);
            }
            else
            {
                GUISystem::SetFocusFree(false);
            }
        }
        return space;
    }
}

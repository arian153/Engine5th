#include "Level.hpp"
#include "../Space/Space.hpp"
#include "../../System/Physics/Dynamics/World.hpp"
#include "../../System/Graphics/Element/Scene.hpp"
#include "../Object/ObjectManager.hpp"
#include "../Object/Object.hpp"
#include "../Component/EngineComponent/TransformComponent.hpp"

namespace Engine5
{
    Level::Level(const std::string& name)
    {
    }

    Level::~Level()
    {
    }

    void Level::Initialize() const
    {
    }

    void Level::Update(Real dt) const
    {
        /*for (auto& space : m_spaces)
        {
            auto obj_manager = space->GetObjectManager();
            if (obj_manager != nullptr)
            {
                auto found = obj_manager->FindObjectBegin("SpecularMapCube");
                if (found != nullptr && found->HasComponent<TransformComponent>())
                {
                    auto transform = found->GetComponent<TransformComponent>();
                    if (transform != nullptr)
                    {
                        transform->AddRotationY(dt);
                    }
                }
            }
        }*/
    }

    void Level::FixedUpdate(Real dt) const
    {
    }

    void Level::Shutdown() const
    {
    }

    void Level::Load() const
    {
    }

    void Level::Unload() const
    {
    }

    Space* Level::GetGlobalSpace() const
    {
        return m_global_space;
    }

    Space* Level::GetSpace(size_t index) const
    {
        return m_spaces.at(index);
    }

    void Level::UpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const
    {
        auto space = m_spaces.at(index);
        UpdateSpace(dt, space, flag);
    }

    void Level::UpdateSubsystem(Real dt, eSubsystemFlag flag) const
    {
        if (m_global_space->IsSubsystemUpdate(flag))
        {
            UpdateSpace(dt, m_global_space, flag);
        }
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemUpdate(flag))
            {
                UpdateSpace(dt, space, flag);
            }
        }
    }

    void Level::DrawSubsystem(eSubsystemFlag flag)
    {
        if (m_global_space->IsSubsystemUpdate(flag) || m_global_space->IsSubsystemFixedUpdate(flag))
        {
            DrawSpace(m_global_space, flag);
        }
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemUpdate(flag) || space->IsSubsystemFixedUpdate(flag))
            {
                DrawSpace(space, flag);
            }
        }
    }

    void Level::FixedUpdateSubsystem(Real dt, eSubsystemFlag flag) const
    {
        if (m_global_space->IsSubsystemFixedUpdate(flag))
        {
            UpdateSpace(dt, m_global_space, flag);
        }
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemFixedUpdate(flag))
            {
                UpdateSpace(dt, space, flag);
            }
        }
    }

    void Level::AddSpaceResource(JsonResource* resource)
    {
        if (resource != nullptr)
        {
            m_space_resources.push_back(resource);
        }
    }

    void Level::UpdateSpace(Real dt, Space* space, eSubsystemFlag flag) const
    {
        if (flag == eSubsystemFlag::Scene)
        {
            auto scene = space->GetScene();
            if (scene != nullptr)
            {
                scene->Update(dt);
            }
        }
        else if (flag == eSubsystemFlag::World)
        {
            auto world = space->GetWorld();
            if (world != nullptr)
            {
                world->Update(dt);
            }
        }
    }

    void Level::DrawSpace(Space* space, eSubsystemFlag flag)
    {
        if (flag == eSubsystemFlag::World)
        {
            auto world = space->GetWorld();
            if (world != nullptr)
            {
                world->Draw();
            }
        }
        if (flag == eSubsystemFlag::Scene)
        {
            auto scene = space->GetScene();
            if (scene != nullptr)
            {
                scene->Render();
            }
        }
    }
}

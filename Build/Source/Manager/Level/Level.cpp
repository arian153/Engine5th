#include "Level.hpp"
#include "../Space/Space.hpp"
#include "../../System/Physics/Dynamics/World.hpp"
#include "../../System/Graphics/Element/Scene.hpp"

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

    Space* Level::GetUISpace() const
    {
        return m_ui_space;
    }

    Space* Level::GetWorldSpace() const
    {
        return m_world_space;
    }

    Space* Level::GetSpace(size_t index) const
    {
        return m_spaces.at(index);
    }

    void Level::UpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const
    {
        auto space = m_spaces.at(index);
        if (space->GetScene() != nullptr && HasFlag(flag, eSubsystemFlag::Scene))
        {
            space->GetScene()->Update(dt);
        }
        if (space->GetWorld() != nullptr && HasFlag(flag, eSubsystemFlag::World))
        {
            space->GetWorld()->Update(dt);
        }
    }

    void Level::UpdateSubsystem(Real dt, eSubsystemFlag flag) const
    {
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemUpdate(flag))
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
        }
    }

    void Level::FixedUpdateSubsystem(Real dt, eSubsystemFlag flag)
    {
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemFixedUpdate(flag))
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
        }
    }
}

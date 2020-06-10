#include "Level.hpp"
#include "../Space/Space.hpp"
#include "../../System/Physics/Dynamics/World.hpp"
#include "../../System/Graphics/Element/Scene.hpp"
#include "../../System/Logic/LogicSubsystem.hpp"

namespace Engine5
{
    Level::Level(const std::string& name)
        : m_name(name)
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
        if (flag == eSubsystemFlag::World)
        {
            auto world = space->GetWorld();
            if (world != nullptr)
            {
                world->Update(dt);
            }
        }
        if (flag == eSubsystemFlag::Logic)
        {
            auto logic = space->GetLogicSubsystem();
            if (logic != nullptr)
            {
                logic->Update(dt);
            }
        }
        if (flag == eSubsystemFlag::Scene)
        {
            auto scene = space->GetScene();
            if (scene != nullptr)
            {
                scene->Update(dt);
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
                world->Render();
            }
        }
        if (flag == eSubsystemFlag::Logic)
        {
            auto logic = space->GetLogicSubsystem();
            if (logic != nullptr)
            {
                logic->Render();
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

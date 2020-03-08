#include "Level.hpp"
#include "../Space/Space.hpp"

namespace Engine5
{
    Level::Level(const std::string& name)
    {
    }

    Level::~Level()
    {
    }

    Space* Level::GetGlobalSpace() const
    {
        return m_global_space;
    }

    void Level::Initialize()
    {
    }

    void Level::Update(Real dt) const
    {
        for (auto& space : m_spaces)
        {
            space->Update(dt);
        }
    }

    void Level::FixedUpdate(Real dt) const
    {
        for (auto& space : m_spaces)
        {
            space->FixedUpdate(dt);
        }
    }

    void Level::Shutdown()
    {
    }

    void Level::UpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const
    {
        m_spaces.at(index)->Update(dt, flag);
    }

    void Level::FixedUpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const
    {
        m_spaces.at(index)->FixedUpdate(dt, flag);
    }
}

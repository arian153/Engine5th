#include "LogicSubsystem.hpp"
#include "../../Manager/Component/GameComponent/LogicComponent.hpp"

namespace Engine5
{
    LogicSubsystem::LogicSubsystem()
    {
    }

    LogicSubsystem::~LogicSubsystem()
    {
    }

    void LogicSubsystem::Initialize()
    {
    }

    void LogicSubsystem::Update(Real dt)
    {
        for (auto& logic : m_logic_components)
        {
            logic->Update(dt);
        }
    }

    void LogicSubsystem::Render() const
    {
        for (auto& logic : m_logic_components)
        {
            logic->Render();
        }
    }

    void LogicSubsystem::Shutdown()
    {
        for (auto& logic : m_logic_components)
        {
            logic->Shutdown();
            delete logic;
            logic = nullptr;
        }
        m_logic_components.clear();
    }

    void LogicSubsystem::AddLogic(LogicComponent* logic)
    {
        m_logic_components.push_back(logic);
    }

    void LogicSubsystem::RemoveLogic(LogicComponent* logic)
    {
        auto found = std::find(m_logic_components.begin(), m_logic_components.end(), logic);
        m_logic_components.erase(found);
    }

    void LogicSubsystem::InitializeLogic(LogicComponent* logic) const
    {
    }
}

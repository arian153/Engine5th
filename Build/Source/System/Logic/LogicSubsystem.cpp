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
        for(auto& logic : m_logic_components)
        {
            logic->Update(dt);
        }
    }

    void LogicSubsystem::Render() const
    {
    }

    void LogicSubsystem::Shutdown()
    {
    }

    void LogicSubsystem::AddLogic(LogicComponent* logic)
    {
    }

    void LogicSubsystem::RemoveLogic(LogicComponent* logic)
    {
    }

    void LogicSubsystem::InitializeLogic(LogicComponent* logic) const
    {
    }
}

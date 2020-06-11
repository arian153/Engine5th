#pragma once
#include <vector>

namespace Engine5
{
    class InputCommon;
    class LogicSubsystem;

    class LogicSystem
    {
    public:
        LogicSystem();
        ~LogicSystem();

        void Initialize();
        void Shutdown();

        void SetInput(InputCommon* input);

        LogicSubsystem* CreateLogicSubsystem();
        void   RemoveLogicSubsystem(LogicSubsystem* logic_subsystem);

    private:
        std::vector<LogicSubsystem*> m_subsystems;
        InputCommon*                 m_input = nullptr;
    };
}

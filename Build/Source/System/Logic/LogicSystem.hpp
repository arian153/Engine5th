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
        
    private:
        std::vector<LogicSubsystem*> m_subsystems;
        InputCommon* m_input = nullptr;
    };
}

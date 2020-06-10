#pragma once
#include <vector>
#include "../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class InputCommon;
    class LogicComponent;

    class LogicSubsystem
    {
    public:
        LogicSubsystem();
        ~LogicSubsystem();

        void Initialize();
        void Update(Real dt);
        void Render() const;
        void Shutdown();

        void AddLogic(LogicComponent* logic);
        void RemoveLogic(LogicComponent* logic);
        void InitializeLogic(LogicComponent* logic) const;

    private:
        std::vector<LogicComponent*> m_logic_components;

        //
        InputCommon* m_input = nullptr;
    };
}

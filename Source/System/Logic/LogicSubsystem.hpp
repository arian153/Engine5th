#pragma once
#include <vector>
#include "../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class PrimitiveRenderer;
    class TextRenderer;
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
        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);

    private:
        friend class LogicSystem;
    private:
        std::vector<LogicComponent*> m_logic_components;

        //
        InputCommon*       m_input              = nullptr;
        TextRenderer*      m_text_renderer      = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;
    };
}

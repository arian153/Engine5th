#pragma once
#include "..//Component.hpp"

namespace Engine5
{
    class LogicComponent : public Component
    {
    public:
        virtual ~LogicComponent();
        LogicComponent()                                     = delete;
        LogicComponent(const LogicComponent& rhs)            = delete;
        LogicComponent& operator=(const LogicComponent& rhs) = delete;

    protected:
        explicit     LogicComponent(Object* owner);

    protected:
    };
}

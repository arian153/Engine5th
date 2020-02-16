#pragma once
#include <string>

namespace Engine5
{
    class Component;
    class Object;

    class ComponentFactory
    {
    public:
        ComponentFactory()
        {
        }

        virtual ~ComponentFactory()
        {
        }

        virtual Component* Create(Object* owner) = 0;
        virtual Component* Clone(Component* origin, Object* dest) = 0;

    protected:
        friend class ComponentRegistry;
        std::string type;
    };
}

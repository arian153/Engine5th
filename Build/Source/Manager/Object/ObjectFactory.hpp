#pragma once
#include <vector>
#include <string>

namespace Engine5
{
    class Object;
    class ObjectManager;

    class ObjectFactory
    {
    public:
        ObjectFactory();
        ~ObjectFactory();

        void Init();
        void Shutdown();

        Object* CreateRawObject(const std::string& name = "Empty Object", ObjectManager* object_manager = nullptr);
        Object* CreateArchetypeObject(size_t archetype_id, const std::string& name = "", ObjectManager* object_manager = nullptr);

    private:
        void ClearArchetypes();

    private:
        std::vector<Object*> m_archetypes;
    };
}

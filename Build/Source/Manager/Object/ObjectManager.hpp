#pragma once
#include <unordered_map>

namespace Engine5
{
    class Object;

    class ObjectManager
    {
    public:
        ObjectManager();
        ~ObjectManager();

        Object* AddObject(const std::string& name, Object* created = nullptr);

        void    FindObjects(const std::string& name, std::vector<Object*>& objects);
        Object* FindObjectBegin(const std::string& name);
        Object* FindObjectEnd(const std::string& name);
        Object* FindObjectAt(size_t id);

        void RemoveObjects(const std::string& name);
        void RemoveObject(const std::string& name);
        void RemoveObjectAt(size_t id);
        void RemoveObject(Object* object);

        Object* EraseObject(Object* object);
        Object* EraseObjectAt(size_t id);
        void ClearObjects();

        bool HasObject(const std::string& name);
        bool HasObject(Object* object);

    private:
        void ChangeName(Object* object, const std::string& new_name);
        void ResetID();

    private:
        std::unordered_multimap<std::string, Object*> m_object_map;
        std::vector<Object*>                          m_objects;
    };
}

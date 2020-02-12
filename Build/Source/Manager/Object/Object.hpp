#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine5
{
    class Component;

    class Object
    {
    public:
        Object();
        ~Object();

        Object* GetChildAt(size_t index) const;
        Object* GetSiblingAt(size_t index) const;
        Object* GetParent() const;
        Object* GetAncestor() const;

        std::vector<Object*>* GetChildren() const;
        std::vector<Object*>* GetSibling() const;

        void AddChild(Object* baby_child);
        void ClearFamily();

    private:
        inline static size_t s_id = 0;

    private:
        size_t      m_id;
        std::string m_name;

        //family
        Object*               m_ancestor = nullptr;
        Object*               m_parent   = nullptr;
        std::vector<Object*>* m_children = nullptr;
        std::vector<Object*>* m_sibling  = nullptr;

        //components
        std::unordered_map<std::string, Component*> m_components;
    };
}

#include "ObjectTemplate.cpp"

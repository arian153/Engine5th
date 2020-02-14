#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine5
{
    class ComponentManager;
    class ObjectFactory;
    class ObjectManager;
    class Component;

    class Object
    {
    public:
        Object();
        ~Object();

        size_t GetID() const;

        std::string GetName() const;
        void        SetName(const std::string& name);

        Object* Clone(ObjectManager* manager = nullptr);

        Object* GetChildAt(size_t index) const;
        Object* GetSiblingAt(size_t index) const;
        Object* GetParent() const;
        Object* GetAncestor() const;

        std::vector<Object*>* GetChildren() const;
        std::vector<Object*>* GetSibling() const;

        void AddChild(Object* baby_child);
        void RemoveChild(Object* child) const;
        void RemoveSibling(Object* sibling) const;
        void ClearObjectHierarchy();
        void ReleaseObjectHierarchy();
        void DestroyObjectHierarchy();
        void DestroyFamilyRecursive();

        void ClearComponents();

    public: //template member functions
        template <typename T>
        T* AddComponent();

        template <typename T>
        bool HasComponent() const;

        template <typename T>
        T* GetComponent() const;

        template <typename T>
        void RemoveComponent();

    private:
        void SetManager(ObjectManager* obj_manager, ComponentManager* cmp_manager);
        void SetFactory(ObjectFactory* factory);

        Component* AddComponent(Component* component);
        Component* AddComponent(const std::string& type);
        Component* GetComponent(const std::string& type);

        bool HasComponent(const std::string& type);
        void RemoveComponent(Component* component);
        void RemoveComponent(const std::string& type);

    private:
        inline static size_t s_id = 0;

    private:
        size_t         m_id;
        std::string    m_name;
        ObjectManager* m_manager = nullptr;

        //family
        Object*               m_ancestor = nullptr;
        Object*               m_parent   = nullptr;
        std::vector<Object*>* m_children = nullptr;
        std::vector<Object*>* m_sibling  = nullptr;

        //components
        std::unordered_map<std::string, Component*> m_components;
        ComponentManager* m_component_manager = nullptr;
    };
}

// ReSharper disable once CppUnusedIncludeDirective
#include "ObjectTemplate.cpp"

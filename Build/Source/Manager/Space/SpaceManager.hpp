#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include "SpaceFlag.hpp"
#include <vector>

namespace Engine5
{
    class Space;
    class ObjectFactory;
    class ComponentRegistry;
    class PhysicsSystem;
    class RenderSystem;

    class SpaceManager
    {
    public:
        SpaceManager() = delete;
        explicit SpaceManager(PhysicsSystem* physics, RenderSystem* renderer, ObjectFactory* obj, ComponentRegistry* cmp);
        ~SpaceManager();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Activate(Space* space);
        void Deactivate(Space* space);

        void SetGlobalOrder(bool b_first);

        Space* CreateSpace(eSubsystemFlag flag);
        void RemoveSpace(Space* space);

    private:
        void UpdateFirst(Real dt);
        void UpdateLast(Real dt);

    private:
        friend class Space;

    private:
        bool m_b_global_first = true;
        bool m_b_next_order   = true;

        Space*              m_global_space  = nullptr;
        std::vector<Space*> m_active_spaces;
        std::vector<Space*> m_inactive_spaces;
        //systems

        PhysicsSystem* m_physics_system = nullptr;
        RenderSystem*  m_render_system  = nullptr;

        //factory
        ObjectFactory*    m_object_factory    = nullptr;
        ComponentRegistry* m_component_registry = nullptr;
    };
}

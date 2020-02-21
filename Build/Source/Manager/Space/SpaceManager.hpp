#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include <vector>

namespace Engine5
{
    class Space;
    class ObjectFactory;
    class ComponentFactory;
    class PhysicsSystem;
    class RenderSystem;

    class SpaceManager
    {
    public:
        SpaceManager() = delete;
        explicit SpaceManager(PhysicsSystem* physics, RenderSystem* renderer, ObjectFactory* obj, ComponentFactory* cmp);
        ~SpaceManager();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Activate(Space* space);
        void Deactivate(Space* space);

        void SetGlobalOrder(bool b_first);

        Space* CreateSpace();
        void RemoveSpace(Space* space);

    private:
        void UpdateFirst(Real dt);
        void UpdateLast(Real dt);

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
        ComponentFactory* m_component_factory = nullptr;
    };
}

#pragma once
#include "../../Math/Math.hpp"
#include "../NarrowPhase/ManifoldTable.hpp"
#include "../Dynamics/RigidBody.hpp"
#include "../Utility/FrictionUtility.hpp"
#include <vector>
#include "Constraints/ContactConstraint.hpp"
#include "../Utility/ConstraintUtility.hpp"

namespace Engine5
{
    class Force;
    class ColorFlag;
    class Constraint;
    class ContactPoint;
    class ContactManifold;

    class Resolution
    {
    public:
        Resolution();
        ~Resolution();
        void Initialize();
        void Shutdown();

        void SolveConstraints(ManifoldTable* manifold_table, std::vector<RigidBody*>* rigid_bodies, Real dt);
        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);
        void Render(const ColorFlag& draw_contact_flag);

    private:
        friend class World;
    private:
        bool m_b_warm_starting = true;

        size_t m_velocity_iteration = 3;
        size_t m_position_iteration = 0;

        FrictionUtility                m_friction_utility;
        ConstraintUtility              m_constraint_utility;
        std::vector<ContactConstraint> m_contact_constraints;
        std::vector<Constraint*>       m_constraints;
        std::vector<Force*>            m_forces;
        PrimitiveRenderer*             m_primitive_renderer = nullptr;
    };
}

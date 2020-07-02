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

        void SetVelocityIteration(size_t iteration);
        void SetPositionIteration(size_t iteration);
        void SetWarmStarting(bool b_warm_starting);
    private:
        bool m_b_warm_starting = true;

        size_t m_velocity_iteration = 8;
        size_t m_position_iteration = 3;

        Physics::FrictionUtility       m_friction;
        ConstraintUtility              m_constraint_utility;
        std::vector<ContactConstraint> m_contact_constraints;
        std::vector<Constraint*>       m_constraints;
        PrimitiveRenderer*             m_primitive_renderer = nullptr;
    };
}

#include "Resolution.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "Constraints/ContactConstraint.hpp"
#include <vector>
// ReSharper disable once CppUnusedIncludeDirective
#include "ContactManifold.hpp"
#include "../../Graphics/DataType/Color.hpp"

namespace Engine5
{
    Resolution::Resolution()
    {
    }

    Resolution::~Resolution()
    {
    }

    void Resolution::Initialize()
    {
        m_friction.Initialize();
    }

    void Resolution::Shutdown()
    {
        m_friction.Shutdown();
    }

    void Resolution::SolveConstraints(ManifoldTable* manifold_table, std::vector<RigidBody*>* rigid_bodies, Real dt)
    {
        //velocity phase
        for (auto& body : *rigid_bodies)
        {
            body->IntegrateVelocity(dt);
        }
        m_contact_constraints.clear();
        for (auto& manifold : manifold_table->m_manifold_table)
        {
            auto& contact = m_contact_constraints.emplace_back(&manifold.second, &m_friction);
            contact.GenerateVelocityConstraints(dt);
        }
        if (m_b_warm_starting == true)
        {
            for (auto& contact_constraint : m_contact_constraints)
            {
                contact_constraint.WarmStart();
            }
        }
        for (size_t i = 0; i < m_velocity_iteration; ++i)
        {
            for (auto& contact : m_contact_constraints)
            {
                contact.SolveVelocityConstraints(dt);
            }
        }
        for (auto& contact : m_contact_constraints)
        {
            contact.ApplyVelocityConstraints();
        }
        //position phase
        for (auto& body : *rigid_bodies)
        {
            body->IntegratePosition(dt);
        }
        for (auto& contact : m_contact_constraints)
        {
            contact.GeneratePositionConstraints(dt);
        }
        for (size_t i = 0; i < m_position_iteration; ++i)
        {
            for (auto& contact : m_contact_constraints)
            {
                contact.SolvePositionConstraints(dt);
            }
        }
        for (auto& contact : m_contact_constraints)
        {
            contact.ApplyPositionConstraints();
        }
    }

    void Resolution::SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer)
    {
        m_primitive_renderer = primitive_renderer;
    }

    void Resolution::Render(const ColorFlag& draw_contact_flag)
    {
        if (draw_contact_flag.b_flag)
        {
            for (auto& contact : m_contact_constraints)
            {
                contact.Render(m_primitive_renderer, draw_contact_flag.color);
            }
        }
    }
}

#include "Resolution.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "Constraints/Constraints.hpp"
#include "Constraints/ContactConstraints.hpp"
#include <vector>
// ReSharper disable once CppUnusedIncludeDirective
#include "ContactManifold.hpp"

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

    void Resolution::Solve(ManifoldTable* manifold_table, std::vector<RigidBody*>* rigid_bodies, Real dt)
    {
        //resolution phase
        //solve contact manifold
        for (auto& manifold : manifold_table->m_manifold_table)
        {
            ContactConstraints contact(&manifold.second, &m_friction);
            contact.Initialize();
            if (m_b_warm_starting == true)
            {
                contact.WarmStart();
            }
            //iterate n
            contact.Solve(dt);
            contact.Apply();
        }
        //integration phase
        for (auto& body : *rigid_bodies)
        {
            body->Integrate(dt);
        }
        //solve position constraints.
        for (auto& manifold : manifold_table->m_manifold_table)
        {
            ContactConstraints::SolvePositionConstraints(manifold.second);
        }
    }

    void Resolution::SolveConstraints(Constraints* constraints, Real dt) const
    {
        constraints->Initialize();
        constraints->Solve(dt);
        constraints->Apply();
    }
}

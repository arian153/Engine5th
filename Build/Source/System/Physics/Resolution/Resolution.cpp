#include "Resolution.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "Constraints/Constraints.hpp"

namespace Engine5
{
    Resolution::Resolution()
    {
    }

    Resolution::~Resolution()
    {
    }

    void Resolution::Solve()
    {
        //resolution phase
        for (auto& manifold : m_manifold_table->m_manifold_table)
        {
            ContactConstraints contact(&manifold.second, 1.0f, 1.0f);
            m_resolution_phase->SolveConstraints(&contact, dt);
        }
        //integration phase
        for (auto& body : m_rigid_bodies)
        {
            body->Integrate(dt);
        }
        //solve position constraints.
        for (auto& manifold : m_manifold_table->m_manifold_table)
        {
            ContactConstraints::SolvePositionConstraints(manifold.second);
        }
    }

    void Resolution::SolveConstraints(Constraints* constraints, Real dt) const
    {
        constraints->InitializeConstraints();
        constraints->SolveConstraints(dt);
        constraints->ApplyConstraints();
    }

}

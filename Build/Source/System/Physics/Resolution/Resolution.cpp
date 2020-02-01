#include "Resolution.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "Constraints/Constraints.hpp"
#include "Constraints/ContactConstraints.hpp"

namespace Engine5
{
    Resolution::Resolution()
    {
    }

    Resolution::~Resolution()
    {
    }

    void Resolution::Solve(ManifoldTable* manifold_table, std::vector<RigidBody*>* rigid_bodies, Real dt) const
    {
        //resolution phase
        //solve contact manifold
        for (auto& manifold : manifold_table->m_manifold_table)
        {
            ContactConstraints contact(&manifold.second, 1.0f, 1.0f);
            contact.InitializeConstraints();

            if(m_b_warm_starting == true)
            {
                contact.WarmStart();
            }

            //iterate n
            contact.SolveConstraints(dt);


            contact.ApplyConstraints();
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
        constraints->InitializeConstraints();
        constraints->SolveConstraints(dt);
        constraints->ApplyConstraints();
    }

}

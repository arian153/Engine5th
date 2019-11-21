#include "ColliderSet.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    ColliderSet::ColliderSet()
    {
    }

    ColliderSet::~ColliderSet()
    {
    }

    void ColliderSet::Initialize()
    {
    }

    void ColliderSet::Update(Real dt)
    {
    }

    void ColliderSet::Shutdown()
    {
    }

    MassData ColliderSet::CalculateMassData()
    {
        MassData result;
        result.mass = 0.0f;
        result.local_centroid.SetZero();
        if (m_colliders != nullptr)
        {
            // compute local centroid & mass
            for (auto& collider_data : *m_colliders)
            {
                // accumulate mass
                result.mass += collider_data->m_mass;

                // accumulate weighted contribution
                result.local_centroid += collider_data->m_mass * collider_data->m_centroid;
            }

            // compute inverse mass
            if (Utility::IsZero(result.mass) == false)
            {
                result.inverse_mass = 1.0f / result.mass;
            }
            else
            {
                //infinite mass.
                result.inverse_mass = 0.0f;
            }

            // compute final local centroid
            result.local_centroid *= result.inverse_mass;

            // compute local inertia tensor
            result.local_inertia_tensor.SetZero();
            for (auto& collider_data : *m_colliders)
            {
                Vector3  r       = result.local_centroid - collider_data->m_centroid;
                Real     r_dot_r = r.DotProduct(r);
                Matrix33 r_out_r = r.OuterProduct(r);

                // accumulate local inertia tensor contribution, using Parallel Axis Theorem
                result.local_inertia_tensor += collider_data->m_local_inertia_tensor + collider_data->m_mass * (r_dot_r * Matrix33::Identity() - r_out_r);
            }

            // compute inverse inertia tensor
            result.local_inverse_inertia_tensor = result.local_inertia_tensor.Inverse();
        }
        m_mass_data = result;
        return result;
    }
}

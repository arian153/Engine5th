#include "ColliderCircle.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderCircle::ColliderCircle()
    {
    }

    ColliderCircle::~ColliderCircle()
    {
    }

    void ColliderCircle::Initialize()
    {
    }

    void ColliderCircle::Shutdown()
    {
    }

    Vector3 ColliderCircle::Support(const Vector3& direction)
    {
        return direction;
    }

    bool ColliderCircle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderCircle::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderCircle::SetMassData(Real density)
    {
        Real r;
        m_density = density;
        m_mass    = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            r = m_transformed_radius;
        }
        else
        {
            r = m_radius;
        }
        Real it_xx = 0.25f * m_mass * r * r;
        Real it_yy = 0.25f * m_mass * r * r;
        Real it_zz = 0.5f * m_mass * r * r;
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid = Vector3::Origin();
    }

    Real ColliderCircle::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return Math::PI * m_transformed_radius * m_transformed_radius;
        }
        return Math::PI * m_radius * m_radius;
    }

    void ColliderCircle::SetScaleData(const Vector3& scale)
    {
    }

    void ColliderCircle::SetUnit()
    {
    }

    void ColliderCircle::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Vector3::Origin(), Vector3::Origin());
    }

    void ColliderCircle::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
    }

    void ColliderCircle::Clone(ColliderPrimitive* cloned)
    {
    }
}

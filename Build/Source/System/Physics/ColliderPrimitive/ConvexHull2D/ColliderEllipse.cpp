#include "ColliderEllipse.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderEllipse::ColliderEllipse()
    {
    }

    ColliderEllipse::~ColliderEllipse()
    {
    }

    void ColliderEllipse::Initialize()
    {
    }

    void ColliderEllipse::Shutdown()
    {
    }

    Vector3 ColliderEllipse::Support(const Vector3& direction)
    {
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Vector2 radius    = Radius();
        Vector2 sub_space_direction(local_dir);
        sub_space_direction.SetNormalize();
        Vector2 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(sub_space_direction);
        result /= radius.HadamardProduct(sub_space_direction).Length();
        return Vector3(result.x, result.y, 0.0f);
    }

    bool ColliderEllipse::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderEllipse::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderEllipse::SetMassData(Real density)
    {
        Real a, b;
        m_density = density;
        m_mass    = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            a = m_scaled_radius.x;
            b = m_scaled_radius.y;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.y;
        }
        Real it_xx = 0.25f * m_mass * b * b;
        Real it_yy = 0.25f * m_mass * a * a;
        Real it_zz = 0.25f * m_mass * (a * a + b * b);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid = Vector3::Origin();
    }

    Real ColliderEllipse::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return Math::PI * m_scaled_radius.x * m_scaled_radius.y;
        }
        return Math::PI * m_radius.x * m_radius.y;
    }

    void ColliderEllipse::SetScaleData(const Vector3& scale)
    {
    }

    void ColliderEllipse::SetUnit()
    {
    }

    void ColliderEllipse::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Vector3::Origin(), Vector3::Origin());
    }

    void ColliderEllipse::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
    }

    Vector2 ColliderEllipse::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderEllipse::Clone(ColliderPrimitive* cloned)
    {
    }
}

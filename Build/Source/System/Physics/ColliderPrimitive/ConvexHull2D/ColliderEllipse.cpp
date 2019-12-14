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
        m_scaled_radius = m_radius.HadamardProduct(Vector2(scale));
        m_scale_factor  = scale.Length();
    }

    void ColliderEllipse::SetUnit()
    {
        m_radius.SetNormalize();
        UpdatePrimitive();
    }

    void ColliderEllipse::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Vector3::Origin(), Vector3::Origin());
    }

    void ColliderEllipse::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = renderer->CIRCULAR_VERTICES_COUNT;
        renderer->ReserveVertices(count, mode);
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Vector2    radius           = Radius();
        Real       radian_step      = Math::TWO_PI / static_cast<Real>(count);
        for (int i = 0; i < count; ++i)
        {
            Real    angle = static_cast<Real>(i) * radian_step;
            Vector3 vertex(cosf(angle) * radius.x, sinf(angle) * radius.y, 0.0f);
            vertex = m_orientation.Rotate(vertex);
            vertex += m_position;
            vertex = body_orientation.Rotate(vertex);
            vertex += body_position;
            renderer->PushVertex(vertex, mode, color);
        }
        if (mode == RenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            renderer->ReserveIndices(200, mode);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushLineIndices(index + i, index + i + 1);
            }
            renderer->PushLineIndices(index + count - 1, index);
        }
        else if (mode == RenderingMode::Face)
        {
            //add a center pos
            I32     center   = static_cast<I32>(renderer->VerticesSize(mode));
            Vector3 position = m_position;
            position         = body_orientation.Rotate(position);
            position += body_position;
            renderer->PushVertex(position, mode, color);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushFaceIndices(center, index + i + 1, index + i);
            }
            renderer->PushFaceIndices(center, index, index + count - 1);
        }
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

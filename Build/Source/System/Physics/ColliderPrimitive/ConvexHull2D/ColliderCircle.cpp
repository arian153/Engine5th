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
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Vector3 subspace_direction = local_dir;
        subspace_direction.z = 0.0f;
        subspace_direction.SetNormalize();
        Vector3 result = Radius() * subspace_direction;
        return LocalToWorldPoint(result);
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
            r = m_scaled_radius;
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
            return Math::PI * m_scaled_radius * m_scaled_radius;
        }
        return Math::PI * m_radius * m_radius;
    }

    void ColliderCircle::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius * scale.Length();
        m_scale_factor = scale.Length();
    }

    void ColliderCircle::SetUnit()
    {
        m_radius = 0.5f;
        UpdatePrimitive();
    }

    void ColliderCircle::UpdateBoundingVolume()
    {
        Real    bounding_factor = m_radius;
        Vector3 pos;
        if (m_rigid_body != nullptr)
        {
            pos = m_rigid_body->LocalToWorldPoint(m_position);
            bounding_factor *= m_scale_factor;
        }
        else
        {
            pos = m_position;
        }
        Vector3 min_max(bounding_factor, bounding_factor, bounding_factor);
        m_bounding_volume->Set(-min_max + pos, min_max + pos);
    }

    void ColliderCircle::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = renderer->CIRCULAR_VERTICES_COUNT;
        renderer->ReserveVertices(count, mode);
        Vector3    body_position = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Real    radius = Radius();
        Real       radian_step = Math::TWO_PI / static_cast<Real>(count);
        for (int i = 0; i < count; ++i)
        {
            Real    angle = static_cast<Real>(i) * radian_step;
            Vector3 vertex(cosf(angle) * radius, sinf(angle) * radius, 0.0f);
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
            I32     center = static_cast<I32>(renderer->VerticesSize(mode));
            Vector3 position = m_position;
            position = body_orientation.Rotate(position);
            position += body_position;
            renderer->PushVertex(position, mode, color);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushFaceIndices(center, index + i + 1, index + i);
            }
            renderer->PushFaceIndices(center, index, index + count - 1);
        }
    }

    Real ColliderCircle::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderCircle::Clone(ColliderPrimitive* cloned)
    {
    }
}

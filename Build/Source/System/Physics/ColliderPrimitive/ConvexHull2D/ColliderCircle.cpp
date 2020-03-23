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
        SetCircle(0.5f);
    }

    void ColliderCircle::Shutdown()
    {
    }

    Vector3 ColliderCircle::Support(const Vector3& direction)
    {
        Vector3 subspace_direction = direction;
        subspace_direction.z       = 0.0f;
        subspace_direction.SetNormalize();
        Vector3 result = Radius() * subspace_direction;
        return result;
    }

    bool ColliderCircle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real radius = Radius();
        minimum_t   = -1.0f;
        maximum_t   = -1.0f;
        //Quadratic elements
        Real    a = local_ray.direction.x * local_ray.direction.x + local_ray.direction.y * local_ray.direction.y;
        Real    b = 2.0f * (local_ray.direction.x * local_ray.position.x + local_ray.direction.y * local_ray.position.y);
        Real    c = (local_ray.position.x * local_ray.position.x + local_ray.position.y * local_ray.position.y) - (radius * radius);
        Real    circle_min_t, circle_max_t;
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Utility::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Utility::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                if (Utility::SolveQuadratic(a, b, c, circle_max_t, circle_min_t))
                {
                    //solve intersection
                    if (Utility::IsEqual(circle_min_t, circle_max_t))
                    {
                        minimum_t = maximum_t = circle_min_t;
                    }
                    else
                    {
                        minimum_t = circle_min_t;
                        maximum_t = circle_max_t;
                    }
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define circle.
            if ((plane_intersection - m_position).LengthSquared() < radius * radius)
            {
                minimum_t = maximum_t = plane_t;
            }
            else
            {
                return false;
            }
        }
        if (minimum_t < 0.0f && maximum_t < 0.0f)
        {
            return false;
        }
        if (minimum_t <= 0.0f)
        {
            minimum_t = 0.0f;
        }
        return true;
    }

    Vector3 ColliderCircle::GetNormal(const Vector3& local_point_on_collider)
    {
        Real radius       = Radius();
        Real point_radius = local_point_on_collider.x * local_point_on_collider.x + local_point_on_collider.y * local_point_on_collider.y;
        if (Utility::IsEqual(point_radius, radius * radius))
        {
            return local_point_on_collider.Normalize();
        }
        return Math::Vector3::Z_AXIS;
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
        m_centroid = Math::Vector3::ORIGIN;
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
        m_scale_factor  = scale.Length();
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

    void ColliderCircle::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = renderer->CIRCULAR_VERTICES_COUNT;
        renderer->ReserveVertices(count, mode);
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Real       radius           = Radius();
        Real       radian_step      = Math::TWO_PI / static_cast<Real>(count);
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
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            renderer->ReserveIndices(200, mode);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushLineIndices(index + i, index + i + 1);
            }
            renderer->PushLineIndices(index + count - 1, index);
        }
        else if (mode == eRenderingMode::Face)
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

    Real ColliderCircle::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderCircle::SetCircle(Real radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderCircle::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderCircle* circle = static_cast<ColliderCircle*>(origin);
            //collider local space data
            m_orientation  = circle->m_orientation;
            m_position     = circle->m_position;
            m_scale_factor = circle->m_scale_factor;
            //collider mass data
            m_centroid             = circle->m_centroid;
            m_mass                 = circle->m_mass;
            m_local_inertia_tensor = circle->m_local_inertia_tensor;
            m_density              = circle->m_density;
            m_material             = circle->m_material;
            //circle
            m_radius        = circle->m_radius;
            m_scaled_radius = circle->m_scaled_radius;
        }
    }
}

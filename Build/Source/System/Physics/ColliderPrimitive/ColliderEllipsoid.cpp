#include "ColliderEllipsoid.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderEllipsoid::ColliderEllipsoid()
    {
        m_type = ColliderType::Ellipsoid;
    }

    ColliderEllipsoid::~ColliderEllipsoid()
    {
    }

    void ColliderEllipsoid::Initialize()
    {
    }

    void ColliderEllipsoid::Shutdown()
    {
    }

    Vector3 ColliderEllipsoid::Support(const Vector3& direction)
    {
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Vector3 radius    = Radius();
        Vector3 result    = radius.HadamardProduct(radius);
        result            = result.HadamardProduct(local_dir);
        result /= radius.HadamardProduct(local_dir).Length();
        return LocalToWorldPoint(result);
    }

    bool ColliderEllipsoid::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Vector3 radius = Radius();
        Real    a      = (local_ray.direction.x * local_ray.direction.x / (radius.x * radius.x))
                + (local_ray.direction.y * local_ray.direction.y / (radius.y * radius.y))
                + (local_ray.direction.z * local_ray.direction.z / (radius.z * radius.z));
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x / (radius.x * radius.x))
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius.y * radius.y))
                + (2.0f * local_ray.direction.z * local_ray.position.z / (radius.z * radius.z));
        Real c = (local_ray.position.x * local_ray.position.x / (radius.x * radius.x))
                + (local_ray.position.y * local_ray.position.y / (radius.y * radius.y))
                + (local_ray.position.z * local_ray.position.z / (radius.z * radius.z)) - 1.0f;
        if (Utility::SolveQuadratic(a, b, c, maximum_t, minimum_t) == true)
        {
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
        return false;
    }

    Vector3 ColliderEllipsoid::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector3 normal;
        if (m_rigid_body != nullptr)
        {
            normal.x = 2.0f * local_point_on_collider.x / m_transformed_radius.x * m_transformed_radius.x;
            normal.y = 2.0f * local_point_on_collider.y / m_transformed_radius.y * m_transformed_radius.y;
            normal.z = 2.0f * local_point_on_collider.z / m_transformed_radius.z * m_transformed_radius.z;
        }
        else
        {
            normal.x = 2.0f * local_point_on_collider.x / m_radius.x * m_radius.x;
            normal.y = 2.0f * local_point_on_collider.y / m_radius.y * m_radius.y;
            normal.z = 2.0f * local_point_on_collider.z / m_radius.z * m_radius.z;
        }
        return normal;
    }

    void ColliderEllipsoid::SetMassData(Real density)
    {
        Real a, b, c;
        m_mass = density * GetVolume();
        if (m_rigid_body != nullptr)
        {
            a = m_transformed_radius.x;
            b = m_transformed_radius.z;
            c = m_transformed_radius.y;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.z;
            c = m_radius.y;
        }
        Real it_xx = 0.2f * m_mass * (b * b + c * c);
        Real it_yy = 0.2f * m_mass * (a * a + b * b);
        Real it_zz = 0.2f * m_mass * (a * a + c * c);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid.SetZero();
    }

    Real ColliderEllipsoid::GetVolume()
    {
        if (m_rigid_body != nullptr)
        {
            return 4.0f / 3.0f * Math::PI * m_transformed_radius.x * m_transformed_radius.y * m_transformed_radius.z;
        }
        return 4.0f / 3.0f * Math::PI * m_radius.x * m_radius.y * m_radius.z;
    }

    void ColliderEllipsoid::UpdateScale(const Vector3& scale)
    {
        m_transformed_radius = m_radius.HadamardProduct(scale);
        m_scale_factor       = scale.Length();
    }

    void ColliderEllipsoid::SetUnit()
    {
        m_radius.SetNormalize();
        if (m_collider_set != nullptr)
        {
            UpdateScale(m_collider_set->GetTransformScale());
            m_collider_set->CalculateMassData();
        }
    }

    void ColliderEllipsoid::UpdateBoundingVolume()
    {
        Real    bounding_factor = m_radius.Length();
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

    void ColliderEllipsoid::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3    axis_vector      = Vector3::AxisY();
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Vector3    radius           = Radius();

        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos.HadamardProduct(radius);

        //modify rotation, translation
        top_vertex_local_pos = m_orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += m_position;
        top_vertex_local_pos = body_orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += body_position;

        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos.HadamardProduct(radius);

        //modify rotation, translation
        bottom_vertex_local_pos = m_orientation.Rotate(bottom_vertex_local_pos);
        bottom_vertex_local_pos += m_position;
        bottom_vertex_local_pos = body_orientation.Rotate(bottom_vertex_local_pos);
        bottom_vertex_local_pos += body_position;
        renderer->PushVertex(top_vertex_local_pos, mode, color);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;

        // Compute vertices for each stack ring (do not count the poles as rings).
        for (I32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;

                // spherical to cartesian
                Vector3 vertex_local_pos;
                vertex_local_pos.x = sinf(phi) * cosf(theta);
                vertex_local_pos.y = cosf(phi);
                vertex_local_pos.z = sinf(phi) * sinf(theta);
                vertex_local_pos   = vertex_local_pos.HadamardProduct(radius);
                vertex_local_pos   = m_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += m_position;
                vertex_local_pos = body_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += body_position;
                renderer->PushVertex(vertex_local_pos, mode, color);
            }
        }
        renderer->PushVertex(bottom_vertex_local_pos, mode, color);
        if (mode == RenderingMode::Dot)
        {
            I32 count = renderer->SPHERICAL_VERTICES_COUNT;
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushLineIndices(index, index + i + 1);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(base + i * ring + j, base + i * ring + j + 1);
                    renderer->PushLineIndices(base + i * ring + j, base + (i + 1) * ring + j);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushLineIndices(south_pole_index, base + i);
            }
        }
        else if (mode == RenderingMode::Face)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushFaceIndices(index, index + i + 1, index + i);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushFaceIndices(
                                              base + i * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j);
                    renderer->PushFaceIndices(
                                              base + (i + 1) * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j + 1);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushFaceIndices(south_pole_index, base + i, base + i + 1);
            }
        }
    }

    Vector3 ColliderEllipsoid::Radius() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_transformed_radius;
        }
        return m_radius;
    }

    void ColliderEllipsoid::SetEllipsoid(const Vector3& radius)
    {
        m_radius = radius;
        if (m_collider_set != nullptr)
        {
            UpdateScale(m_collider_set->GetTransformScale());
            m_collider_set->CalculateMassData();
        }
    }

    void ColliderEllipsoid::Clone(ColliderPrimitive* cloned)
    {
    }
}

#include "ColliderDome.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderDome::ColliderDome()
    {
        m_type = eColliderType::Dome;
    }

    ColliderDome::~ColliderDome()
    {
    }

    void ColliderDome::Initialize()
    {
        SetDome(Vector3(0.5f, 0.5f, 0.5f));
    }

    void ColliderDome::Shutdown()
    {
    }

    Vector3 ColliderDome::Support(const Vector3& direction)
    {
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Vector3 radius    = Radius();
        Vector3 result    = radius.HadamardProduct(radius);
        result            = result.HadamardProduct(local_dir);
        result /= radius.HadamardProduct(local_dir).Length();
        //check hemisphere
        if (result.y <= 0.0f)
        {
            Vector3 ellipse_dir = local_dir;
            ellipse_dir.y       = 0.0f;
            ellipse_dir.SetNormalize();
            Vector3 ellipse_radius = radius;
            ellipse_radius.y       = 0.0f;
            result                 = ellipse_radius.HadamardProduct(ellipse_radius);
            result                 = result.HadamardProduct(ellipse_dir);
            result /= ellipse_radius.HadamardProduct(ellipse_dir).Length();
        }
        return LocalToWorldPoint(result);
    }

    bool ColliderDome::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Vector3 radius        = Radius();
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        //quadratic
        Real a = (local_ray.direction.x * local_ray.direction.x * denominator_x)
                + (local_ray.direction.y * local_ray.direction.y / (radius.y * radius.y))
                + (local_ray.direction.z * local_ray.direction.z * denominator_z);
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x * denominator_x)
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius.y * radius.y))
                + (2.0f * local_ray.direction.z * local_ray.position.z * denominator_z);
        Real c = (local_ray.position.x * local_ray.position.x * denominator_x)
                + (local_ray.position.y * local_ray.position.y / (radius.y * radius.y))
                + (local_ray.position.z * local_ray.position.z * denominator_z) - 1.0f;
        if (Utility::SolveQuadratic(a, b, c, maximum_t, minimum_t) == true)
        {
            Real max_height = local_ray.direction.y * maximum_t + local_ray.position.y;
            Real min_height = local_ray.direction.y * minimum_t + local_ray.position.y;
            if (max_height < 0.0f)
            {
                if (min_height < 0.0f)
                {
                    return false;
                }
                Real disc_t = -local_ray.position.y / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    maximum_t = disc_t;
                }
            }
            if (min_height < 0.0f)
            {
                Real disc_t = -local_ray.position.y / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
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
        return false;
    }

    Vector3 ColliderDome::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderDome::SetMassData(Real density)
    {
        Real a, b, c;
        m_density = density;
        m_mass    = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            a = m_scaled_radius.x;
            b = m_scaled_radius.z;
            c = m_scaled_radius.y;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.z;
            c = m_radius.y;
        }
        //0.059375 = 19/320
        Real it_xx = 0.2f * m_mass * b * b + 0.059375f * m_mass * c * c;
        Real it_zz = 0.2f * m_mass * a * a + 0.059375f * m_mass * c * c;
        Real it_yy = 0.2f * m_mass * (a * a + b * b);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid.Set(0.0f, 0.375f * c, 0.0f);
    }

    Real ColliderDome::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return 2.0f / 3.0f * Math::PI * m_scaled_radius.x * m_scaled_radius.y * m_scaled_radius.z;
        }
        return 2.0f / 3.0f * Math::PI * m_radius.x * m_radius.y * m_radius.z;
    }

    void ColliderDome::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius.HadamardProduct(scale);
        m_scale_factor       = scale.Length();
    }

    void ColliderDome::SetUnit()
    {
        m_radius.SetNormalize();
        UpdatePrimitive();
    }

    void ColliderDome::UpdateBoundingVolume()
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

    void ColliderDome::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index               = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count         = renderer->SPHERICAL_STACK_COUNT + 1; //for equal divide.
        int slice_count         = renderer->SPHERICAL_SLICE_COUNT;
        I32 half_stack_count    = stack_count / 2;
        I32 dome_vertices_count = (stack_count / 2) * (slice_count + 1) + 1;
        renderer->ReserveVertices((size_t)dome_vertices_count, mode);
        Vector3    axis_vector      = Math::Vector3::Y_AXIS;
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
        renderer->PushVertex(top_vertex_local_pos, mode, color);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;

        // Compute vertices for each stack ring (do not count the poles as rings).
        for (I32 i = 1; i <= half_stack_count; ++i)
        {
            Real phi = i * phi_step;
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
        I32 base = 1 + index;
        I32 ring = slice_count + 1;
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < dome_vertices_count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushLineIndices(index, index + i + 1);
            }
            for (I32 i = 0; i < half_stack_count; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(base + i * ring + j, base + i * ring + j + 1);
                    if (i < half_stack_count - 1)
                    {
                        renderer->PushLineIndices(base + i * ring + j, base + (i + 1) * ring + j);
                    }
                }
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushFaceIndices(index, index + i + 1, index + i);
            }
            for (I32 i = 0; i < half_stack_count - 1; ++i)
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
        }
    }

    Vector3 ColliderDome::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderDome::SetDome(const Vector3& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderDome::Clone(ColliderPrimitive* cloned)
    {
    }
}

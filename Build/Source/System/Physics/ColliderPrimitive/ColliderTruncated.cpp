#include "ColliderTruncated.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    Vector3 ColliderTruncated::Support(const Vector3& direction)
    {
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Vector2 radius    = Radius();
        Vector3 axis_vector;
        axis_vector.y = HalfHeight();
        Vector3 result;
        Vector3 ellipse_dir = local_dir;
        ellipse_dir.y       = 0.0f;
        ellipse_dir.SetNormalize();
        Vector3 ellipse_radius(radius.x, 0.0f, radius.y);
        Vector3 ellipse_vector = ellipse_radius.HadamardProduct(ellipse_radius);
        ellipse_vector         = ellipse_vector.HadamardProduct(ellipse_dir);
        ellipse_vector /= ellipse_radius.HadamardProduct(ellipse_dir).Length();
        Real top_support = local_dir.DotProduct(m_ratio * ellipse_vector + axis_vector);
        Real bot_support = local_dir.DotProduct(ellipse_vector - axis_vector);
        if (top_support > bot_support)
        {
            result = m_ratio * ellipse_vector + axis_vector;
        }
        else
        {
            result = ellipse_vector - axis_vector;
        }
        return LocalToWorldPoint(result);
    }

    bool ColliderTruncated::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    half_height     = Height() * 0.5f;
        Vector2 radius          = Radius();
        Real    height          = Height();
        minimum_t               = -1.0f;
        maximum_t               = -1.0f;
        Real denominator_x      = 1.0f / (radius.x * radius.x);
        Real denominator_y      = 1.0f / (height * height);
        Real denominator_z      = 1.0f / (radius.y * radius.y);
        Real ratio_multiplier_a = (m_ratio - 1.0f) * (m_ratio - 1.0f);
        Real ratio_multiplier_b = (m_ratio + 1.0f) * (m_ratio + 1.0f);
        Real ratio_multiplier_c = m_ratio * m_ratio - 1.0f;
        //make quadratic
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x
                + local_ray.direction.z * local_ray.direction.z * denominator_z
                - local_ray.direction.y * local_ray.direction.y * ratio_multiplier_a * denominator_y;
        Real b = 2.0f * local_ray.direction.x * local_ray.position.x * denominator_x
                + 2.0f * local_ray.direction.z * local_ray.position.z * denominator_z
                - 2.0f * local_ray.direction.y * local_ray.position.y * ratio_multiplier_a * denominator_y
                - local_ray.direction.y * ratio_multiplier_c / height;
        Real c = local_ray.position.x * local_ray.position.x * denominator_x
                + local_ray.position.z * local_ray.position.z * denominator_z
                - local_ray.position.y * local_ray.position.y * ratio_multiplier_a * denominator_y
                - local_ray.position.y * ratio_multiplier_c / height
                - 0.25f * ratio_multiplier_b;
        Real truncated_min_t, truncated_max_t;
        if (Utility::SolveQuadratic(a, b, c, truncated_max_t, truncated_min_t) == true)
        {
            if (truncated_min_t > truncated_max_t)
            {
                Real temp       = truncated_min_t;
                truncated_min_t = truncated_max_t;
                truncated_max_t = temp;
            }
            Real min_axis_height = local_ray.position.y + local_ray.direction.y * truncated_min_t;
            Real max_axis_height = local_ray.position.y + local_ray.direction.y * truncated_max_t;
            minimum_t            = truncated_min_t;
            maximum_t            = truncated_max_t;
            if (min_axis_height > half_height)
            {
                if (max_axis_height > half_height)
                {
                    return false;
                }
                Real disc_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= m_ratio * m_ratio)
                {
                    minimum_t = disc_t;
                }
                else
                {
                    //return result;
                }
            }
            if (max_axis_height < -half_height)
            {
                if (min_axis_height < -half_height)
                {
                    return false;
                }
                Real disc_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                }
                else
                {
                }
            }
            if (max_axis_height > half_height)
            {
                Real disc_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= m_ratio * m_ratio)
                {
                    //minimum_t = truncated_min_t;
                    maximum_t = disc_t;
                }
                else
                {
                    return false;
                }
            }
            if (min_axis_height < -half_height)
            {
                Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                    //maximum_t = truncated_max_t;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            if (m_ratio < 1.0f)
            {
                Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                    maximum_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                    if (minimum_t > maximum_t)
                    {
                        Real temp = minimum_t;
                        minimum_t = maximum_t;
                        maximum_t = temp;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                Real disc_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= m_ratio * m_ratio)
                {
                    minimum_t = disc_t;
                    maximum_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                    if (minimum_t > maximum_t)
                    {
                        Real temp = minimum_t;
                        minimum_t = maximum_t;
                        maximum_t = temp;
                    }
                }
                else
                {
                    return false;
                }
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

    Vector3 ColliderTruncated::GetNormal(const Vector3& local_point_on_collider)
    {
        Real    half_height   = HalfHeight();
        Vector2 radius        = Radius();
        Real    height        = Height();
        Real    n_ratio       = (m_ratio + 1.0f) * 0.5f + local_point_on_collider.y * (m_ratio - 1.0f) / height;
        Real    x_max         = local_point_on_collider.x / n_ratio;
        Real    z_max         = local_point_on_collider.z / n_ratio;
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_y = 1.0f / (height * height);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        Real    t             = 2.0f * (1.0f - m_ratio) * (local_point_on_collider.x * x_max * denominator_x + local_point_on_collider.z * z_max * denominator_z) * denominator_y;
        Real    point_radius  = local_point_on_collider.x * local_point_on_collider.x * denominator_x + local_point_on_collider.z * local_point_on_collider.z * denominator_z;
        Vector3 normal;
        if (Utility::IsEqual(local_point_on_collider.y, half_height) && point_radius < m_ratio * m_ratio)
        {
            normal.y = Utility::Signum(local_point_on_collider.y);
        }
        else if (Utility::IsEqual(local_point_on_collider.y, -half_height) && point_radius < 1.0f)
        {
            normal.y = Utility::Signum(local_point_on_collider.y);
        }
        else
        {
            normal.x = 2.0f * local_point_on_collider.x * denominator_x;
            normal.y = height * t;
            normal.z = 2.0f * local_point_on_collider.z * denominator_z;
            normal.SetNormalize();
        }
        return normal;
    }

    void ColliderTruncated::CalculateMassData(Real density)
    {
        Real a, b, h;
        m_mass             = density * GetVolume();
        Real ratio         = (m_ratio * m_ratio + m_ratio + 1.0f);
        Real ratio_multi_a = (m_ratio * m_ratio * m_ratio * m_ratio + m_ratio * m_ratio * m_ratio + m_ratio * m_ratio + m_ratio + 1.0f) / ratio;
        Real ratio_multi_b = (m_ratio * m_ratio * m_ratio * m_ratio + 4.0f * m_ratio * m_ratio * m_ratio + 10.0f * m_ratio * m_ratio + 4.0f * m_ratio + 1.0f) / (ratio * ratio);
        if (m_rigid_body != nullptr)
        {
            a = m_transformed_radius.x;
            b = m_transformed_radius.y;
            h = m_transformed_height;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.y;
            h = m_height;
        }
        Real it_xx = 0.15f * m_mass * b * b * ratio_multi_a + 0.0375f * m_mass * h * h * ratio_multi_b;
        Real it_zz = 0.15f * m_mass * a * a * ratio_multi_a + 0.0375f * m_mass * h * h * ratio_multi_b;
        Real it_yy = 0.15f * m_mass * (a * a + b * b) * ratio_multi_a;
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid = Vector3(0.0f, (3.0f * m_ratio * m_ratio + 2.0f * m_ratio + 1.0f) * h / (4.0f * ratio), 0.0f) - HalfHeight();
    }

    Real ColliderTruncated::GetVolume()
    {
        Real ratio = m_ratio * m_ratio + m_ratio + 1.0f;
        if (m_rigid_body != nullptr)
        {
            return Math::PI * m_transformed_radius.x * m_transformed_radius.y * m_transformed_height * ratio / 3.0f;
        }
        return Math::PI * m_radius.x * m_radius.y * m_height * ratio / 3.0f;
    }

    void ColliderTruncated::ScalePrimitiveData(const Vector3& scale)
    {
        m_transformed_height = m_height * scale.y;
        m_transformed_radius = m_radius.HadamardProduct(Vector2(scale.x, scale.z));
    }


    void ColliderTruncated::MakeUnitPrimitive()
    {
        m_radius = 0.5f;
        m_height = 1.0f;

        //TODO - get scale from transform 
        Vector3 scale(1.0f, 1.0f, 1.0f);
        ScalePrimitiveData(scale);
    }

    void ColliderTruncated::UpdateBoundingVolume()
    {
    }

    void ColliderTruncated::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color)
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->CYLINDRICAL_STACK_COUNT;
        int slice_count = renderer->CYLINDRICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->CYLINDRICAL_VERTICES_COUNT, mode);
        Real       height           = Height();
        Vector2    radius           = Radius();
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Real       stack_height     = height / stack_count;
        Real       radius_step      = (m_ratio - 1.0f) / stack_count;
        I32        ring_count       = stack_count + 1;
        for (I32 i = 0; i < ring_count; ++i)
        {
            Real y       = -0.5f * height + i * stack_height;
            Real r       = 1.0f + i * radius_step;
            Real d_theta = Math::TWO_PI / slice_count;
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real    c = cosf(j * d_theta);
                Real    s = sinf(j * d_theta);
                Vector3 vertex_local_pos;
                vertex_local_pos.x = radius.x * r * c;
                vertex_local_pos.y = y;
                vertex_local_pos.z = radius.y * r * s;
                vertex_local_pos   = m_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += m_position;
                vertex_local_pos = body_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += body_position;
                renderer->PushVertex(vertex_local_pos, mode, color);
            }
        }
        I32 ring = slice_count + 1;
        if (mode == RenderingMode::Dot)
        {
            I32 count = renderer->CYLINDRICAL_VERTICES_COUNT;
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            for (I32 i = 0; i < stack_count; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(index + (i * ring + j), index + ((i + 1) * ring + j));
                    renderer->PushLineIndices(index + ((i + 1) * ring + j), index + ((i + 1) * ring + j + 1));
                    renderer->PushLineIndices(index + (i * ring + j), index + (i * ring + j + 1));
                    renderer->PushLineIndices(index + ((i + 1) * ring + j + 1), index + (i * ring + j + 1));
                }
            }
        }
        else if (mode == RenderingMode::Face)
        {
            for (I32 i = 0; i < stack_count; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushFaceIndices(index + (i * ring + j), index + ((i + 1) * ring + j), index + (i + 1) * ring + j + 1);
                    renderer->PushFaceIndices(index + (i * ring + j), index + ((i + 1) * ring + j + 1), index + (i * ring + j + 1));
                }
            }
        }
    }

    Real ColliderTruncated::HalfHeight() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_transformed_height * 0.5f;
        }
        return m_height * 0.5f;
    }

    Real ColliderTruncated::Height() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_transformed_height;
        }
        return m_height;
    }

    Vector2 ColliderTruncated::Radius() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_transformed_radius;
        }
        return m_radius;
    }

    void ColliderTruncated::Clone(ColliderPrimitive* cloned)
    {
    }
}

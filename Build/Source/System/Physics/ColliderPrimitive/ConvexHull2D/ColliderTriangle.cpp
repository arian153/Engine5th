#include "ColliderTriangle.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderTriangle::ColliderTriangle()
    {
    }

    ColliderTriangle::~ColliderTriangle()
    {
    }

    void ColliderTriangle::Initialize()
    {
    }

    void ColliderTriangle::Shutdown()
    {
    }

    Vector3 ColliderTriangle::Support(const Vector3& direction)
    {

        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 3; ++i)
        {
            Real projection = Vector3(Vertex(i)).DotProduct(local_dir);
            if (projection > p)
            {
                result = Vertex(i);
                p = projection;
            }
        }
        return LocalToWorldPoint(result);
    }

    bool ColliderTriangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderTriangle::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderTriangle::SetMassData(Real density)
    {
        Real area  = 0.0f;
        Real it_xx = 0.0f;
        Real it_yy = 0.0f;
        Real it_zz = 0.0f;
        Real inv3  = 1.0f / 3.0f;
        for (size_t i1 = 0; i1 < 3; ++i1)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  i2 = i1 + 1 < 3 ? i1 + 1 : 0;
            if (m_collider_set != nullptr)
            {
                p1 = Vector3(m_scaled_vertices[i1].x, m_scaled_vertices[i1].y);
                p2 = Vector3(m_scaled_vertices[i2].x, m_scaled_vertices[i2].y);
            }
            else
            {
                p1 = Vector3(m_vertices[i1].x, m_vertices[i1].y);
                p2 = Vector3(m_vertices[i2].x, m_vertices[i2].y);
            }
            Real triangle_area = 0.5f * p1.CrossProduct(p2).Length();
            area += triangle_area;

            // Use area to weight the centroid average, not just vertex position
            m_centroid += (p1 + p2) * triangle_area * inv3;
            Real it_x = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
            Real it_y = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
            it_xx += 0.25f * inv3 * p1.CrossProduct(p2).Length() * it_y;
            it_yy += 0.25f * inv3 * p1.CrossProduct(p2).Length() * it_x;
            it_zz += 0.25f * inv3 * p1.CrossProduct(p2).Length() * (it_x + it_y);
        }
        m_centroid *= 1.0f / area;
        m_mass = density * area;
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx * density, it_yy * density, it_zz * density);
    }

    Real ColliderTriangle::GetVolume()
    {
        Vector3 edge01, edge02;
        if (m_collider_set != nullptr)
        {
            auto v2edge01 = m_scaled_vertices[1] - m_scaled_vertices[0];
            auto v2edge02 = m_scaled_vertices[2] - m_scaled_vertices[0];
            edge01        = Vector3(v2edge01.x, v2edge01.y, 0.0f);
            edge02        = Vector3(v2edge02.x, v2edge02.y, 0.0f);
        }
        else
        {
            auto v2edge01 = m_vertices[1] - m_vertices[0];
            auto v2edge02 = m_vertices[2] - m_vertices[0];
            edge01        = Vector3(v2edge01.x, v2edge01.y, 0.0f);
            edge02        = Vector3(v2edge02.x, v2edge02.y, 0.0f);
        }
        return edge01.CrossProduct(edge02).Length() * 0.5f;
    }

    void ColliderTriangle::SetScaleData(const Vector3& scale)
    {
    }

    void ColliderTriangle::SetUnit()
    {
    }

    void ColliderTriangle::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Vector3::Origin(), Vector3::Origin());
    }

    void ColliderTriangle::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
    }

    Vector2 ColliderTriangle::Vertex(size_t i)
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices[i];
        }
        return m_vertices[i];
    }

    void ColliderTriangle::SetTriangle(const Vector2& v0, const Vector2& v1, const Vector2& v2)
    {
        m_vertices[0] = v0;
        m_vertices[1] = v1;
        m_vertices[2] = v2;
        UpdatePrimitive();
    }

    void ColliderTriangle::Clone(ColliderPrimitive* cloned)
    {
    }
}

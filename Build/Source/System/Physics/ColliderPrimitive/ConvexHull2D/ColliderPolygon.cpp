#include "ColliderPolygon.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderPolygon::ColliderPolygon()
    {
    }

    ColliderPolygon::~ColliderPolygon()
    {
    }

    void ColliderPolygon::Initialize()
    {
    }

    void ColliderPolygon::Shutdown()
    {
    }

    Vector3 ColliderPolygon::Support(const Vector3& direction)
    {
        return direction;
    }

    bool ColliderPolygon::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderPolygon::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderPolygon::SetMassData(Real density)
    {
        Real   area  = 0.0f;
        Real   it_xx = 0.0f;
        Real   it_yy = 0.0f;
        Real   it_zz = 0.0f;
        Real   inv3  = 1.0f / 3.0f;
        size_t size  = m_vertices->size();
        for (size_t i1 = 0; i1 < 3; ++i1)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  i2 = i1 + 1 < size ? i1 + 1 : 0;
            if (m_collider_set != nullptr)
            {
                p1 = Vector3(m_scaled_vertices->at(i1).x, m_scaled_vertices->at(i1).y);
                p2 = Vector3(m_scaled_vertices->at(i2).x, m_scaled_vertices->at(i2).y);
            }
            else
            {
                p1 = Vector3(m_vertices->at(i1).x, m_vertices->at(i1).y);
                p2 = Vector3(m_vertices->at(i2).x, m_vertices->at(i2).y);
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

    Real ColliderPolygon::GetVolume()
    {
        Real   area = 0.0f;
        size_t size = m_vertices->size();
        for (size_t i1 = 0; i1 < 3; ++i1)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  i2 = i1 + 1 < size ? i1 + 1 : 0;
            if (m_collider_set != nullptr)
            {
                p1 = Vector3(m_scaled_vertices->at(i1).x, m_scaled_vertices->at(i1).y);
                p2 = Vector3(m_scaled_vertices->at(i2).x, m_scaled_vertices->at(i2).y);
            }
            else
            {
                p1 = Vector3(m_vertices->at(i1).x, m_vertices->at(i1).y);
                p2 = Vector3(m_vertices->at(i2).x, m_vertices->at(i2).y);
            }
            Real triangle_area = 0.5f * p1.CrossProduct(p2).Length();
            area += triangle_area;
        }
        return area;
    }

    void ColliderPolygon::SetScaleData(const Vector3& scale)
    {
    }

    void ColliderPolygon::SetUnit()
    {
    }

    void ColliderPolygon::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Vector3::Origin(), Vector3::Origin());
    }

    void ColliderPolygon::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
    }

    void ColliderPolygon::Clone(ColliderPrimitive* cloned)
    {
    }
}

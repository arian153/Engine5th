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
        for (size_t i1 = 0; i1 < size; ++i1)
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
        size_t  size = m_vertices->size();
        Vector2 scale_v2(scale.x, scale.y);
        for (size_t i = 0; i < size; ++i)
        {
            m_scaled_vertices->at(i) = scale_v2.HadamardProduct(m_vertices->at(i));
        }
        m_scale_factor = scale.Length();
    }

    void ColliderPolygon::SetUnit()
    {
        auto min_max     = m_max_bound - m_min_bound;
        Real unit_factor = 1.0f / min_max.Length();
        for (auto& vertex : *m_vertices)
        {
            vertex *= unit_factor;
        }
        UpdatePrimitive();
    }

    void ColliderPolygon::UpdateBoundingVolume()
    {
        Real    bounding_factor = (m_max_bound - m_min_bound).Length() * 0.5f;
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

    void ColliderPolygon::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32    index = static_cast<I32>(renderer->VerticesSize(mode));
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }

        size_t size  = vertices->size();
        renderer->ReserveVertices(size, mode);
        Vector3               body_position    = GetBodyPosition();
        Quaternion            body_orientation = GetBodyOrientation();
       
        for (auto& vertex : *vertices)
        {
            //collider local space to object space(body local)
            Vector3 vertex_v3(vertex);
            vertex_v3 = m_orientation.Rotate(vertex_v3);
            vertex_v3 += m_position;

            //body local space to world space
            vertex_v3 = body_orientation.Rotate(vertex_v3);
            vertex_v3 += body_position;
            //push to renderer
            renderer->PushVertex(vertex_v3, mode, color);
        }

        //add indices
        if (mode == RenderingMode::Dot)
        {
            for (I32 i = 0; i < size; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            for (auto& edge : *m_edges)
            {
                renderer->PushLineIndices(index + edge.a, index + edge.b);
            }
        }
        else if (mode == RenderingMode::Face)
        {
            Vector3 vertex_v3(Vector3::Origin());
            vertex_v3 = m_orientation.Rotate(vertex_v3);
            vertex_v3 += m_position;
            vertex_v3 = body_orientation.Rotate(vertex_v3);
            vertex_v3 += body_position;
            renderer->PushVertex(vertex_v3, mode, color);
            for (auto& edge : *m_edges)
            {
                renderer->PushFaceIndices(index + edge.a, index + edge.b, size);
            }
        }
    }

    Vector2 ColliderPolygon::Vertex(size_t i) const
    {
        if (i >= m_vertices->size())
        {
            i = m_vertices->size() - 1;
        }
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices->at(i);
        }
        return m_vertices->at(i);
    }

    void ColliderPolygon::Clone(ColliderPrimitive* cloned)
    {
    }
}

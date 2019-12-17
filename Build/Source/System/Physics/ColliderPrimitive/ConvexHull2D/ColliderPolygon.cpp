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
        Vector3               local_dir = WorldToLocalVector(direction).Unit();
        Real                  p         = Math::REAL_NEGATIVE_MAX;
        Vector3               result;
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            Real projection = Vector3(vertices->at(i)).DotProduct(local_dir);
            if (projection > p)
            {
                result = vertices->at(i);
                p      = projection;
            }
        }
        return LocalToWorldPoint(result);
    }

    bool ColliderPolygon::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //Quadratic elements
        Real polygon_min_t = Math::REAL_POSITIVE_MAX;
        Real polygon_max_t = Math::REAL_NEGATIVE_MAX;

        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Utility::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Utility::IsZero(pc.DotProduct(normal)) == true)
            {
                std::vector<Vector2>* vertices;
                if (m_collider_set != nullptr)
                {
                    vertices = m_scaled_vertices;
                }
                else
                {
                    vertices = m_vertices;
                }

                //ray is on the plane.
                Vector2 dir(local_ray.direction.x, local_ray.direction.y);
                Vector2 pos(local_ray.position.x, local_ray.position.y);
                Real    inv_dir = 1.0f / dir.DotProduct(dir);
                size_t  size    = vertices->size();
                for (size_t i = 0; i < size; ++i)
                {
                    size_t  j    = i + 1 < size ? i + 1 : 0;
                    Vector2 p0   = vertices->at(i);
                    Vector2 p1   = vertices->at(j);
                    Vector2 edge = p1 - p0;
                    if (Utility::IsZero(dir.CrossProduct(edge)) == true)
                    {
                        if (Utility::IsZero((p0 - pos).CrossProduct(dir)) == true)
                        {
                            polygon_min_t = (p0 - pos).DotProduct(dir) * inv_dir;
                            polygon_max_t = (p1 - pos).DotProduct(dir) * inv_dir;
                            break;
                        }
                    }
                    else
                    {
                        Real t = (p0 - pos).CrossProduct(edge) / (dir.CrossProduct(edge));
                        if (polygon_min_t > t)
                        {
                            polygon_min_t = t;
                        }
                        if (polygon_max_t < t)
                        {
                            polygon_max_t = t;
                        }
                    }
                }
                //find min max t.
                minimum_t = polygon_min_t;
                maximum_t = polygon_max_t;
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
            //define ellipse.
            bool is_rectangle_contain_point = false;
            if (is_rectangle_contain_point)
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

    Vector3 ColliderPolygon::GetNormal(const Vector3& local_point_on_collider)
    {
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t  size = vertices->size();
        Vector2 v(local_point_on_collider);
        for (size_t i = 0; i < size; ++i)
        {
            size_t  j    = i + 1 < size ? i + 1 : 0;
            Vector2 p0   = vertices->at(i);
            Vector2 p1   = vertices->at(j);
            Vector2 edge = p1 - p0;
            if (p0.IsEqual(p1) == false)
            {
                Real tx = (v.x - p0.x) / edge.x;
                Real ty = (v.y - p0.y) / edge.y;
                Real t  = Utility::IsEqual(tx, ty) ? tx : (!Utility::IsZero(tx) ? tx : ty);

                //is point on edge ?
                if (t <= 1.0f && t >= 0.0f)
                {
                    return Vector3(-v.y, v.x);
                }
            }
        }
        return Math::Vector3::Z_AXIS;
    }

    void ColliderPolygon::SetMassData(Real density)
    {
        Real area  = 0.0f;
        Real it_xx = 0.0f;
        Real it_yy = 0.0f;
        Real it_zz = 0.0f;
        Real inv3  = 1.0f / 3.0f;
        //vertices
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  j          = i + 1 < size ? i + 1 : 0;
            p1                 = Vector3(vertices->at(i).x, vertices->at(i).y);
            p2                 = Vector3(vertices->at(j).x, vertices->at(j).y);
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
        Real area = 0.0f;
        //vertices
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  j = i + 1 < size ? i + 1 : 0;
            p1        = Vector3(vertices->at(i).x, vertices->at(i).y);
            p2        = Vector3(vertices->at(j).x, vertices->at(j).y);
            //area
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
        I32                   index = static_cast<I32>(renderer->VerticesSize(mode));
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        renderer->ReserveVertices(size, mode);
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
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
                renderer->PushLineIndices(index + (I32)edge.a, index + (I32)edge.b);
            }
        }
        else if (mode == RenderingMode::Face)
        {
            Vector3 vertex_v3 = Math::Vector3::ORIGIN;
            vertex_v3 = m_orientation.Rotate(vertex_v3);
            vertex_v3 += m_position;
            vertex_v3 = body_orientation.Rotate(vertex_v3);
            vertex_v3 += body_position;
            renderer->PushVertex(vertex_v3, mode, color);
            for (auto& edge : *m_edges)
            {
                renderer->PushFaceIndices(index + (I32)edge.a, index + (I32)edge.b, (I32)size);
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

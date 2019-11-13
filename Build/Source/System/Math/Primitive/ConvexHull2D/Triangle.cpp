#include "Triangle.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    Triangle::Triangle()
    {
        type = PrimitiveType::Triangle;
    }

    Triangle::~Triangle()
    {
    }

    void Triangle::Initialize()
    {
    }

    void Triangle::Shutdown()
    {
    }

    void Triangle::SetUnit()
    {
        vertices[0] = Vector2(0.0f, 0.0f);
        vertices[1] = Vector2(1.0f, 0.0f);
        vertices[2] = Vector2(0.0f, 1.0f);
    }

    Vector3 Triangle::Support(const Vector3& direction)
    {
        Vector2 sub_space_direction;
        sub_space_direction.x = direction.x;
        sub_space_direction.y = direction.y;
        sub_space_direction.SetNormalize();
        Vector2 result;
        Real    p = Math::REAL_NEGATIVE_MAX;
        for (size_t i = 0; i < 3; ++i)
        {
            Real projection = vertices[i].DotProduct(sub_space_direction);
            if (projection > p)
            {
                result = vertices[i];
                p      = projection;
            }
        }
        return Vector3(result.x, result.y, 0.0f);
    }

    bool Triangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //Quadratic elements
        Real triangle_min_t = Math::REAL_POSITIVE_MAX;
        Real triangle_max_t = Math::REAL_NEGATIVE_MAX;

        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Utility::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Utility::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                Vector2 dir(local_ray.direction.x, local_ray.direction.y);
                Vector2 pos(local_ray.position.x, local_ray.position.y);
                //last edge
                Vector2 p0      = vertices[0];
                Vector2 p1      = vertices[1];
                Vector2 p2      = vertices[2];
                Vector2 edge1   = p1 - p0;
                Vector2 edge2   = p2 - p1;
                Vector2 edge3   = p0 - p2;
                Real    inv_dir = 1.0f / dir.DotProduct(dir);
                if (Utility::IsZero(dir.CrossProduct(edge1)) == true)
                {
                    if (Utility::IsZero((p0 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p0 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p1 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p0 - pos).CrossProduct(edge1) / (dir.CrossProduct(edge1));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
                if (Utility::IsZero(dir.CrossProduct(edge2)) == true)
                {
                    if (Utility::IsZero((p1 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p1 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p2 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p1 - pos).CrossProduct(edge2) / (dir.CrossProduct(edge2));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
                if (Utility::IsZero(dir.CrossProduct(edge3)) == true)
                {
                    if (Utility::IsZero((p2 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p2 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p0 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p2 - pos).CrossProduct(edge3) / (dir.CrossProduct(edge3));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
            }
            else
            {
                return false;
            }
            minimum_t = triangle_min_t;
            maximum_t = triangle_max_t;
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define ellipse.
            Vector2 v2edge1 = vertices[1] - vertices[0];
            Vector2 v2edge2 = vertices[2] - vertices[0];
            Vector3 edge1(v2edge1.x, v2edge1.y);
            Vector3 edge2(v2edge2.x, v2edge2.y);
            Vector3 h = local_ray.direction.CrossProduct(edge2);
            Real    a = edge1.DotProduct(h);
            if (Utility::IsZero(a))
            {
                return false;
            }
            Real    f = 1.0f / a;
            Vector3 s = local_ray.position - Vector3(vertices[0].x, vertices[1].y);
            Real    u = f * (s.DotProduct(h));
            if (u < 0.0f || u > 1.0f)
            {
                return false;
            }
            Vector3 q = s.CrossProduct(edge1);
            Real    v = f * local_ray.direction.DotProduct(q);
            if (v < 0.0f || u + v > 1.0f)
            {
                return false;
            }

            // At this stage we can compute t to find out where the intersection point is on the line.
            triangle_min_t = f * edge2.DotProduct(q);
            if (triangle_min_t > Math::EPSILON) // ray intersection
            {
                minimum_t = maximum_t = triangle_min_t;
            }
            else // This means that there is a line intersection but not a ray intersection.
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

    Vector3 Triangle::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

    void Triangle::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color)
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = 3;
        renderer->ReserveVertices(count, mode);
        Vector3 world_vertices[3];
        for (size_t i = 0; i < 3; ++i)
        {
            //local space to world space
            Vector3 vertex(vertices[i].x, vertices[i].y);
            vertex = orientation.Rotate(vertex);
            vertex += position;

            //push to renderer
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
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushLineIndices(index + i, index + i + 1);
            }
            renderer->PushLineIndices(index + count - 1, index);
        }
        else if (mode == RenderingMode::Face)
        {
            renderer->PushFaceIndices(index + 0, index + 1, index + 2);
        }
    }
}

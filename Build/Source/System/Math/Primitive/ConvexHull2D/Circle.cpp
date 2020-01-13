#include "Circle.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    Circle::Circle()
    {
        type = PrimitiveType::Circle;

    }

    Circle::~Circle()
    {
    }

    void Circle::Initialize()
    {
    }

    void Circle::Shutdown()
    {
    }

    void Circle::SetUnit()
    {
        radius = 0.5f;
    }

    Vector3 Circle::Support(const Vector3& direction)
    {
        Vector3 subspace_direction = direction;
        subspace_direction.z       = 0.0f;
        subspace_direction.SetNormalize();
        return radius * subspace_direction;
    }

    bool Circle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
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
            if ((plane_intersection - position).LengthSquared() < radius * radius)
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

    Vector3 Circle::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real point_radius = local_point_on_primitive.x * local_point_on_primitive.x + local_point_on_primitive.y * local_point_on_primitive.y;
        if (Utility::IsEqual(point_radius, radius * radius))
        {
            return local_point_on_primitive.Unit();;
        }
        return Math::Vector3::Z_AXIS;
    }

    void Circle::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = renderer->CIRCULAR_VERTICES_COUNT;
        renderer->ReserveVertices(count, mode);
        Real radian_step = Math::TWO_PI / static_cast<Real>(count);
        for (int i = 0; i < count; ++i)
        {
            Real    angle = static_cast<Real>(i) * radian_step;
            Vector3 vertex(cosf(angle) * radius, sinf(angle) * radius, 0.0f);
            vertex = orientation.Rotate(vertex);
            vertex += position;
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
            renderer->ReserveIndices(static_cast<size_t>(200), mode);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushLineIndices(index + i, index + i + 1);
            }
            renderer->PushLineIndices(index + count - 1, index);
        }
        else if (mode == RenderingMode::Face)
        {
            I32 center = static_cast<I32>(renderer->VerticesSize(mode));
            renderer->PushVertex(position, mode, color);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushFaceIndices(center, index + i + 1, index + i);
            }
            renderer->PushFaceIndices(center, index, index + count - 1);
        }
    }
}

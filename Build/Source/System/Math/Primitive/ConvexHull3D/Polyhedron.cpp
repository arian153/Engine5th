#include "Polyhedron.hpp"
#include "../../Utility/Utility.hpp"

namespace Engine5
{
    Polyhedron::Polyhedron()
    {
        type = PrimitiveType::Polyhedron;
    }

    Polyhedron::~Polyhedron()
    {
    }

    void Polyhedron::Initialize()
    {
    }

    void Polyhedron::Shutdown()
    {
    }

    void Polyhedron::SetUnit()
    {
    }

    Vector3 Polyhedron::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (auto& vertex : *vertices)
        {
            Real projection = vertex.DotProduct(direction);
            if (projection > p)
            {
                result = vertex;
                p      = projection;
            }
        }
        return result;
    }

    bool Polyhedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 Polyhedron::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

    void Polyhedron::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color)
    {
    }
}

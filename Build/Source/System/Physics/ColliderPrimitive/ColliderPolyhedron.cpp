#include "ColliderPolyhedron.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    ColliderPolyhedron::ColliderPolyhedron()
    {
    }

    ColliderPolyhedron::~ColliderPolyhedron()
    {
    }

    Vector3 ColliderPolyhedron::Support(const Vector3& direction)
    {
        return direction;
    }

    bool ColliderPolyhedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 ColliderPolyhedron::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderPolyhedron::CalculateMassData(Real density)
    {
    }

    Real ColliderPolyhedron::GetVolume()
    {
        return m_mass;
    }

    void ColliderPolyhedron::ScalePrimitiveData(const Vector3& scale)
    {
    }

    void ColliderPolyhedron::MakeUnitPrimitive()
    {
    }

    void ColliderPolyhedron::UpdateBoundingVolume()
    {
    }

    void ColliderPolyhedron::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color)
    {
        if (mode == RenderingMode::Dot)
        {

        }
        else if (mode == RenderingMode::Line)
        {

        }
        else if (mode == RenderingMode::Face)
        {

        }
        else
        {

        }
    }

    void ColliderPolyhedron::Clone(ColliderPrimitive* cloned)
    {
    }
}

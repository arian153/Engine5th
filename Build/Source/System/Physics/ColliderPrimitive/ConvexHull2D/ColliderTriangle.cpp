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
        return direction;
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
    }

    Real ColliderTriangle::GetVolume()
    {
        return 0.0f;
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

    void ColliderTriangle::Clone(ColliderPrimitive* cloned)
    {
    }
}

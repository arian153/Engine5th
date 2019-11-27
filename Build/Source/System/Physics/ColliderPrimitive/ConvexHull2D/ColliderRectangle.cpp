#include "ColliderRectangle.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderRectangle::ColliderRectangle()
    {
    }

    ColliderRectangle::~ColliderRectangle()
    {
    }

    void ColliderRectangle::Initialize()
    {
    }

    void ColliderRectangle::Shutdown()
    {
    }

    Vector3 ColliderRectangle::Support(const Vector3& direction)
    {
        return direction;
    }

    bool ColliderRectangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderRectangle::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderRectangle::SetMassData(Real density)
    {
    }

    Real ColliderRectangle::GetVolume()
    {
        return 0.0f;
    }

    void ColliderRectangle::SetScaleData(const Vector3& scale)
    {
    }

    void ColliderRectangle::SetUnit()
    {
    }

    void ColliderRectangle::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Vector3::Origin(), Vector3::Origin());
    }

    void ColliderRectangle::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
    }

    void ColliderRectangle::Clone(ColliderPrimitive* cloned)
    {
    }
}

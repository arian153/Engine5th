#include "ColliderPlane.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderPlane::ColliderPlane()
    {
    }

    ColliderPlane::~ColliderPlane()
    {
    }

    void ColliderPlane::Initialize()
    {
    }

    void ColliderPlane::Shutdown()
    {
    }

    Vector3 ColliderPlane::Support(const Vector3& direction)
    {
        return direction;
    }

    bool ColliderPlane::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderPlane::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderPlane::SetMassData(Real density)
    {

    }

    Real ColliderPlane::GetVolume()
    {
        return 0.0f;
    }

    void ColliderPlane::SetScaleData(const Vector3& scale)
    {
        m_scale_factor = scale.Length();
    }

    void ColliderPlane::SetUnit()
    {
        UpdatePrimitive();
    }

    void ColliderPlane::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Math::Vector3::ORIGIN, Math::Vector3::ORIGIN);
    }

    void ColliderPlane::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
    }

    void ColliderPlane::Clone(ColliderPrimitive* cloned)
    {
    }
}

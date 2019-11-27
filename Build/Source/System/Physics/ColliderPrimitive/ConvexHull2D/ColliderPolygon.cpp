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
    }

    Real ColliderPolygon::GetVolume()
    {
        return 0.0f;
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

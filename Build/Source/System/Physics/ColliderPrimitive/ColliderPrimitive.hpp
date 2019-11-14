#pragma once
#include "../../Math/Math.hpp"
#include "..//Dynamics/RigidBody.hpp"
#include "../BroadPhase/RayCast.hpp"

namespace Engine5
{
    class Color;
    enum class RenderingMode;
    class PrimitiveRenderer;
    class BoundingAABB;
    class RigidBody;

    class ColliderPrimitive
    {
    public:
        ColliderPrimitive();
        virtual ~ColliderPrimitive();

        void CastRay(RayCastResult& result, Real max_distance = -1.0f);
        void IntersectRay(RayIntersectionResult& result, Real max_distance = -1.0f);

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

        Vector3       GetBodyPosition() const;
        Quaternion    GetBodyOrientation() const;
        RigidBody*    GetRigidBody() const;
        BoundingAABB* GetBoundingVolume() const;

        Vector3 ConvertBodyWorldPoint(const Vector3& local_point) const;
    public:
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        //minkowski support - gjk, epa
        virtual Vector3 Support(const Vector3& direction) = 0;

        //ray - collider intersection
        virtual bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const = 0;
        virtual Vector3 GetNormal(const Vector3& local_point_on_collider) = 0;

        //physics
        virtual void SetMassData(Real density) = 0;
        virtual Real GetVolume() = 0;
        

        //collider 
        virtual void UpdateScale(const Vector3& scale) = 0;
        virtual void SetUnit() = 0;
        virtual void UpdateBoundingVolume() = 0;
        virtual void Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const = 0;

    protected:
        virtual void Clone(ColliderPrimitive* cloned) = 0;

    private:
        friend class RigidBody;
        friend class NarrowPhase;
        friend class BoundingAABB;
        friend class World;

    protected:
        //collider local space data
        Quaternion m_orientation;
        Vector3    m_position;

        //collider mass data
        Vector3  m_centroid; //center of mass
        Real     m_mass;
        Matrix33 m_local_inertia_tensor;

        //other data
        RigidBody*    m_rigid_body      = nullptr;
        BoundingAABB* m_bounding_volume = nullptr;
        ColliderType  m_type;
    };
}

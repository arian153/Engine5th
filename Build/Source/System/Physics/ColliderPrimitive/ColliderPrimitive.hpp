#pragma once
#include "../../Math/Math.hpp"
#include "..//Dynamics/RigidBody.hpp"
#include "../BroadPhase/RayCast.hpp"
#include "../Dynamics/ColliderSet.hpp"


namespace Engine5
{
    class Color;
    enum class RenderingMode;
    class PrimitiveRenderer;
    class BoundingAABB;
    class RigidBody;
    class Transform;

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

        Matrix33 WorldInertia() const;
        Vector3 WorldCentroid() const;

        Vector3       GetBodyPosition() const;
        Quaternion    GetBodyOrientation() const;
        RigidBody*    GetRigidBody() const;
        BoundingAABB* GetBoundingVolume() const;
        Real          GetDensity() const;

        Vector3 ConvertBodyWorldPoint(const Vector3& local_point) const;
        void    SetScale(const Vector3& scale);
        void    SetMass(Real density);
    public:
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        //minkowski support - gjk, epa
        virtual Vector3 Support(const Vector3& direction) = 0;

        //ray - collider intersection
        virtual bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const = 0;
        virtual Vector3 GetNormal(const Vector3& local_point_on_collider) = 0;

        //physics
        virtual Real GetVolume() = 0;

        //collider 
        virtual void SetUnit() = 0;
        virtual void UpdateBoundingVolume() = 0;
        virtual void Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const = 0;

    protected:
        void UpdatePrimitive();
        void UpdateMassData() const;
        void UpdateScaleData();

    protected:
        virtual void SetMassData(Real density) = 0;
        virtual void SetScaleData(const Vector3& scale) = 0;
        virtual void Clone(ColliderPrimitive* cloned) = 0;


    private:
        friend class ColliderSet;
        friend class RigidBody;
        friend class NarrowPhase;
        friend class BoundingAABB;
        friend class World;

    protected:
        //collider local space data
        Quaternion m_orientation;
        Vector3    m_position;
        Real       m_scale_factor = 1.0f;

        //collider mass data
        Vector3  m_centroid; //center of mass
        Real     m_mass;
        Matrix33 m_local_inertia_tensor;
        Real     m_density = 1.0f;

        //other data
        RigidBody*    m_rigid_body      = nullptr;
        ColliderSet*  m_collider_set    = nullptr;
        BoundingAABB* m_bounding_volume = nullptr;
        ColliderType  m_type;
    };
}

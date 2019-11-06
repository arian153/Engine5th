#pragma once
#include "ColliderPrimitive.hpp"

namespace Engine5
{
    /**
     * \brief 
     * Capsule Collider is aligned on the y-axis
     */
    class ColliderCapsule : public ColliderPrimitive
    {
    public:
        //minkowski support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //ray - collider intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_collider) override;

        //physics
        void CalculateMassData(Real density) override;
        Real GetVolume() override;

        //collider 
        void ScalePrimitiveData(const Vector3& scale) override;
        void MakeUnitPrimitive() override;
        void UpdateBoundingVolume() override;
        void DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) override;

        Real    HalfHeight() const;
        Real    Height() const;
        Vector3 Radius() const;

    private:
        bool TestRayEllipsoid(const Ray& ray, const Vector3& centroid, Real& min_t, Real& max_t) const;

    private:
        Vector3 m_radius;
        Vector3 m_transformed_radius;
        Real    m_height             = 1.0f;
        Real    m_transformed_height = 1.0f;
    };
}

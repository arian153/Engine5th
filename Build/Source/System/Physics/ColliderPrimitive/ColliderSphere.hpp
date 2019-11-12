#pragma once
#include "ColliderPrimitive.hpp"

namespace Engine5
{
    class ColliderSphere final : public ColliderPrimitive
    {
    public:
        ColliderSphere();
        ~ColliderSphere() override;
        ColliderSphere(const ColliderSphere& rhs)            = delete;
        ColliderSphere& operator=(const ColliderSphere& rhs) = delete;

        void Initialize() override;
        void Shutdown() override;

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

        Real Radius() const;

    protected:
        void Clone(ColliderPrimitive* cloned) override;

    private:
        Real m_radius             = 1.0f;
        Real m_transformed_radius = 1.0f;
    };
}

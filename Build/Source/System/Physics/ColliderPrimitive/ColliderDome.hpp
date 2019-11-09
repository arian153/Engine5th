#pragma once
#include "ColliderPrimitive.hpp"

namespace Engine5
{
    /**
     * \brief
     * Dome Collider is aligned on the y-axis
     */
    class ColliderDome : public ColliderPrimitive
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

        Vector3 Radius() const;

    protected:
        void Clone(ColliderPrimitive* cloned) override;

    private:
        Vector3 m_radius = Vector3(0.3f, 0.4f, 0.5f);
        Vector3 m_transformed_radius;
    };
}

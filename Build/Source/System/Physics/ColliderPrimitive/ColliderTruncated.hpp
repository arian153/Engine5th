#pragma once
#include "ColliderPrimitive.hpp"

namespace Engine5
{
    /**
     * \brief
     * Truncated Collider is aligned on the y-axis
     */
    class ColliderTruncated final : public ColliderPrimitive
    {
    public:
        //default param
        ColliderTruncated();
        ~ColliderTruncated() override;
        ColliderTruncated(const ColliderTruncated& rhs) = delete;
        ColliderTruncated& operator=(const ColliderTruncated& rhs) = delete;

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
        Vector2 Radius() const;

    protected:
        void Clone(ColliderPrimitive* cloned) override;

    private:
        Vector2 m_radius = Vector2(0.5f, 0.3f);
        Vector2 m_transformed_radius;
        Real    m_height             = 1.0f;
        Real    m_transformed_height = 1.0f;
        Real    m_ratio              = 0.5f;
    };
}

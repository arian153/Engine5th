#pragma once
#include "../Primitive.hpp"
#include "../../Algebra/Vector2.hpp"

namespace Engine5
{
    class Triangle final : public Primitive
    {
    public:
        Triangle();
        ~Triangle();

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

        //Draw
        void DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const override;

        void SetTriangle(const Vector2& v0, const Vector2& v1, const Vector2& v2);
        void SetTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);

        Vector3 Vertex(size_t i) const;
        Vector3 Center() const;
        Vector3 ClosestPoint(const Vector3& point) const;

        Real Distance(const Vector3& point) const;
        Real DistanceSquared(const Vector3& point) const;

    public:
        static Vector3 ClosestPoint(const Vector3& point, const Vector3& v0, const Vector3& v1, const Vector3& v2);
        static Real DistanceSquared(const Vector3& point, const Vector3& v0, const Vector3& v1, const Vector3& v2);
        static Real Distance(const Vector3& point, const Vector3& v0, const Vector3& v1, const Vector3& v2);
        static bool IsContainPoint(const Vector3& point, const Vector3& v0, const Vector3& v1, const Vector3& v2);
        static Vector3 Normal(const Vector3& v0, const Vector3& v1, const Vector3& v2);

    public:
        Vector2 vertices[3];
        Real z_factor = 0.0f;
    };
}

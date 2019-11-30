#pragma once
#include "../ColliderPrimitive.hpp"
#include "../ColliderEdge.hpp"
#include "../ColliderFace.hpp"

namespace Engine5
{
    class ColliderPolyhedron final : public ColliderPrimitive
    {
    public:
        ColliderPolyhedron();
        ~ColliderPolyhedron() override;
        ColliderPolyhedron(const ColliderPolyhedron& rhs)            = delete;
        ColliderPolyhedron& operator=(const ColliderPolyhedron& rhs) = delete;

        void Initialize() override;
        void Shutdown() override;

        //minkowski support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //ray - collider intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_collider) override;

        //physics
        void SetMassData(Real density) override;
        Real GetVolume() override;

        //collider 
        void SetScaleData(const Vector3& scale) override;
        void SetUnit() override;
        void UpdateBoundingVolume() override;
        void Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const override;

    protected:
        void Clone(ColliderPrimitive* cloned) override;

    private:
        std::vector<Vector3>*      m_vertices;
        std::vector<Vector3>*      m_scaled_vertices;
        std::vector<ColliderEdge>* m_edges;
        std::vector<ColliderFace>* m_feces;

        Real    m_volume;
        Vector3 m_min_vertex;
        Vector3 m_max_vertex;
    };
}

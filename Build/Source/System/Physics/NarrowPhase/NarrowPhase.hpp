#pragma once
#include "SupportPoint.hpp"
#include "Simplex.hpp"
#include "../Resolution/Contact.hpp"

namespace Engine5
{
    class ColliderPrimitive;
    class Polytope;

    class NarrowPhase
    {
    public:
        NarrowPhase();
        ~NarrowPhase();

        void GenerateContact(bool b_draw);

        SupportPoint GenerateCSOSupport(ColliderPrimitive* a, ColliderPrimitive* b, const Vector3& direction);
        bool         GJKCollisionDetection(ColliderPrimitive* a, ColliderPrimitive* b, Simplex& simplex);
        bool         EPAContactGeneration(ColliderPrimitive* a, ColliderPrimitive* b, Polytope& polytope, Contact result);

    private:
        void ComputeBasisQuaternion(const Vector3& normal, Vector3& tangent_a, Vector3& tangent_b);
        void ComputeBasisCrossProduct(const Vector3& normal, Vector3& tangent_a, Vector3& tangent_b);
        void ComputeBasisFast(const Vector3& normal, Vector3& tangent_a, Vector3& tangent_b);

    private:
        Vector3 m_search_dirs[6];
        Vector3 m_axes[3];
    };
}

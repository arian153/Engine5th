#pragma once
#include "../../Math/Primitive/Others/Plane.hpp"

namespace Engine5
{
    class Matrix44;

    class Frustum
    {
    public:
        Frustum();
        ~Frustum();

        void ConstructFrustum(Real depth, const Matrix44& projection, const Matrix44& view);

        bool IntersectionPoint(Real x, Real y, Real z) const;
        bool IntersectionCube(Real x, Real y, Real z, Real scale) const;
        bool IntersectionSphere(Real x, Real y, Real z, Real radius) const;
        bool IntersectionRectangle(Real x, Real y, Real z, Real scale_x, Real scale_y, Real scale_z) const;

        Vector3 GetVertex(size_t i);

        Plane  operator[](size_t i) const;
        Plane& operator[](size_t i);

    private:
        Plane m_planes[ 6 ];
    };
}

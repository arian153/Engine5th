#pragma once
#include "SupportPoint.hpp"

namespace Engine5
{
    class Simplex
    {
    public:
        Simplex();
        ~Simplex();
        SupportPoint& operator[](size_t index);
        SupportPoint  operator[](size_t index) const;

        bool DoSimplexPoint(Vector3& dir);
        bool DoSimplexLine(Vector3& dir);
        bool DoSimplexTriangle(Vector3& dir);
        bool DoSimplexTetrahedron(Vector3& dir);

        bool IsContainOrigin(Vector3& dir);

        void Push(const SupportPoint& p);

        void Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c, const SupportPoint& d);
        void Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c);
        void Set(const SupportPoint& a, const SupportPoint& b);
        void Set(const SupportPoint& a);

        bool UpdateSimplexTriangle(Vector3& dir);
        bool UpdateSimplexTetrahedron(Vector3& dir);

    private:
        friend class NarrowPhase;
        friend class Polytope;
        friend class IntersectionUtility;

    private:
        SupportPoint simplex_vertex_a; //point
        SupportPoint simplex_vertex_b; //line_segment
        SupportPoint simplex_vertex_c; //triangle
        SupportPoint simplex_vertex_d; //tetrahedron

        int count = 0;
    };
}

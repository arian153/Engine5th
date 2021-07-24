#pragma once
#include "../../Core/DataStructure/DataStructureDef.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace Engine5
{
    class MeshVertexIndexInfo
    {
    public:
        MeshVertexIndexInfo()  = default;
        ~MeshVertexIndexInfo() = default;

    public:
        size_t point_index = 0, texture_index = 0, normal_index = 0;
    };

    class MeshFaceIndexInfo
    {
    public:
        MeshFaceIndexInfo()
            : vertex_index_a(0), vertex_index_b(0), vertex_index_c(0), uv_index_a(0), uv_index_b(0), uv_index_c(0), normal_index_a(0), normal_index_b(0), normal_index_c(0)
        {
        }

        ~MeshFaceIndexInfo()
        {
        }

    public:
        size_t vertex_index_a, vertex_index_b, vertex_index_c;
        size_t uv_index_a,     uv_index_b,     uv_index_c;
        size_t normal_index_a, normal_index_b, normal_index_c;
    };

    class GeometryFaceIndex
    {
    public:
        GeometryFaceIndex()  = default;
        ~GeometryFaceIndex() = default;

        GeometryFaceIndex(size_t _a, size_t _b, size_t _c)
            : a(_a), b(_b), c(_c)
        {
        }

    public:
        size_t a = 0, b = 0, c = 0;
    };

    class GeometryPointIndex
    {
    public:
        GeometryPointIndex()  = delete;
        ~GeometryPointIndex() = default;

        explicit GeometryPointIndex(size_t i)
            : index(i)
        {
        }

    public:
        size_t                         index = 0;
        std::vector<GeometryFaceIndex> faces;
    };

    class FaceNormal
    {
    public:
        FaceNormal()  = delete;
        ~FaceNormal() = default;

        FaceNormal(size_t _a, size_t _b, size_t _c)
        {
            index.a = _a;
            index.b = _b;
            index.c = _c;
        }

        FaceNormal(size_t _a, size_t _b, size_t _c, const Vector3& n, const Vector3& c)
            : normal(n), center(c)
        {
            index.a = _a;
            index.b = _b;
            index.c = _c;
        }

        explicit FaceNormal(const GeometryFaceIndex& i)
            : normal(), center()
        {
            index.a = i.a;
            index.b = i.b;
            index.c = i.c;
        }

        FaceNormal(const GeometryFaceIndex& i, const Vector3& n, const Vector3& c)
            : normal(n), center(c)
        {
            index.a = i.a;
            index.b = i.b;
            index.c = i.c;
        }

    public:
        GeometryFaceIndex index;
        Vector3           normal;
        Vector3           center;
    };
}

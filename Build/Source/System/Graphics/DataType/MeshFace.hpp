#pragma once

namespace Engine5
{
    class MeshFace
    {
    public:
        MeshFace()
            : vertex_index_a(0), vertex_index_b(0), vertex_index_c(0), uv_index_a(0), uv_index_b(0), uv_index_c(0), normal_index_a(0), normal_index_b(0), normal_index_c(0)
        {
        }

        ~MeshFace()
        {
        }

    public:
        size_t vertex_index_a, vertex_index_b, vertex_index_c;
        size_t uv_index_a,     uv_index_b,     uv_index_c;
        size_t normal_index_a, normal_index_b, normal_index_c;
    };
}

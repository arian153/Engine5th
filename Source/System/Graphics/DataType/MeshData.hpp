#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include <vector>

#include "../Common/Vertex/ColorVertexCommon.hpp"
#include "../Common/Vertex/NormalVertexCommon.hpp"
#include "../Common/Vertex/VertexCommon.hpp"
#include "../Common/Vertex/TextureVertexCommon.hpp"

namespace Engine5
{
    class MeshData
    {
    public:
        MeshData();
        ~MeshData();

        void Convert(std::vector<ColorVertexCommon>& new_vertices, const Color& color);
        void Convert(std::vector<TextureVertexCommon>& new_vertices);
        void Convert(std::vector<NormalVertexCommon>& new_vertices);

    public:
        std::vector<VertexCommon> vertices;
        std::vector<U32>          indices;
        bool                      b_resource = false;
        U32                       stride     = sizeof(VertexCommon);
    };
}

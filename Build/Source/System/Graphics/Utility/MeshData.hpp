#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../Vertex/TextureVertex.hpp"
#include <vector>

namespace Engine5
{
    class MeshData
    {
    public:
        MeshData()
        {
        }

        ~MeshData()
        {
        }

    public:
        std::vector<TextureVertex>   vertices;
        std::vector<U32> indices;
        bool             b_resource = false;
    };
}

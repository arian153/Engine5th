#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "Vertex.hpp"
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
        std::vector<Vertex>   vertices;
        std::vector<U32> indices;
        bool             b_resource = false;
    };
}

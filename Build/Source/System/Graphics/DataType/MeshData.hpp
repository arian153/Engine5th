#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include <vector>
#include "../Vertex/NormalVertex.hpp"

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
        std::vector<NormalVertex> vertices;
        std::vector<U32>           indices;
        bool                       b_resource = false;
    };
}

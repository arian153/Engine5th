#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeMeshBufferAPI
#include "../../Core/Utility/CoreDef.hpp"
#include "../Vertex/ColorVertex.hpp"
#include "../Vertex/NormalVertex.hpp"
#include "../DataType/TopologyDef.hpp"
#include <vector>


namespace Engine5
{
    class RendererCommon;

    class MeshBufferCommon : public MeshBufferAPI
    {
    public:
        MeshBufferCommon();
        ~MeshBufferCommon();

        void Render(U32 stride, U32 offset, eTopologyType topology) const;
        void Render(U32 stride, U32 offset) const;
        void Shutdown();

        bool BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertex>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<NormalVertex>& vertices, const std::vector<U32>& indices);
    private:
    };
}

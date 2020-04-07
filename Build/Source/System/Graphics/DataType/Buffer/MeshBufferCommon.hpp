#pragma once
#include "../../../Core/Utility/CoreDef.hpp"

#include <vector>
#include "../../DataType/Vertex/ColorVertex.hpp"
#include "../../DataType/Vertex/TextureVertex.hpp"
#include "../../DataType/TopologyDef.hpp"
#include "../../Renderer/RendererAPI.hpp"
#include IncludeMeshBufferAPI

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
        bool BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertex>& vertices, const std::vector<U32>& indices);
    private:
    };
}

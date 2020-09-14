#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeMeshBufferAPI
#include "../../Core/Utility/CoreDef.hpp"
#include "../Vertex/ColorVertexCommon.hpp"
#include "../Vertex/TextureVertexCommon.hpp"
#include "../Vertex/NormalVertexCommon.hpp"
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

        bool BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, const std::vector<U32>& indices);
    private:
    };
}

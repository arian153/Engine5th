#pragma once

#include "../API/GraphicsAPI.hpp"
#include IncludeDynamicMeshBufferAPI
#include "../../Core/Utility/CoreDef.hpp"
#include <vector>
#include "../Vertex/NormalVertexCommon.hpp"
#include "../Vertex/ColorVertexCommon.hpp"
#include "../Vertex/TextureVertexCommon.hpp"

namespace Engine5
{
    class RendererCommon;

    class DynamicMeshBufferCommon : public DynamicMeshBufferAPI
    {
    public:
        DynamicMeshBufferCommon();
        ~DynamicMeshBufferCommon();

        void Render(U32 stride, U32 offset) const;
        void Shutdown();

        bool BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, const std::vector<U32>& indices);

        void UpdateVertexBuffer(const std::vector<ColorVertexCommon>& vertices) const;
        void UpdateVertexBuffer(const std::vector<TextureVertexCommon>& vertices) const;
        void UpdateVertexBuffer(const std::vector<NormalVertexCommon>& vertices) const;

    private:
    };
}

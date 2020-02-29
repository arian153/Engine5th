#pragma once
#include "APIMacro.hpp"
#include "DX11/BufferDX11.hpp"
#include "../../Core/Utility/CoreDef.hpp"
#include "../Vertex/ColorVertex.hpp"
#include "../Vertex/TextureVertex.hpp"
#include <vector>

namespace Engine5
{
    class RendererCommon;

    class BufferCommon : public BufferAPI
    {
    public:
        BufferCommon();
        ~BufferCommon();

        void Shutdown();

        bool BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertex>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertex>& vertices, const std::vector<U32>& indices);
    private:
    };
}

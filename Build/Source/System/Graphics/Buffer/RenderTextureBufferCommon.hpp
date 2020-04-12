#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeRenderTextureBufferAPI
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class RendererCommon;

    class RenderTextureBufferCommon : public RenderTextureBufferAPI
    {
    public:
        RenderTextureBufferCommon();
        ~RenderTextureBufferCommon();

        bool Initialize(RendererCommon* renderer, size_t width, size_t height);
        bool Initialize(RendererCommon* renderer, Real left, Real right, Real top, Real bottom);
        void Render() const;
        void Shutdown();
        bool OnResize(size_t width, size_t height);
        bool OnResize(Real left, Real right, Real top, Real bottom);
        U32  GetIndexCount() const;
    private:
    };
}

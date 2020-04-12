#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeRenderTextureBufferAPI
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class RenderTextureBufferCommon : RenderTextureBufferAPI
    {
    public:
        RenderTextureBufferCommon();
        ~RenderTextureBufferCommon();

        bool Initialize(Real width, Real height);
        void Render() const;
        void Shutdown();
        bool OnResize(Real width, Real height);
        U32  GetIndexCount() const;
    private:
    };
}

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
        void Render();
        void Shutdown();
        U32  GetIndexCount() const;
    private:
    };
}

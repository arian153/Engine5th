#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../API/GraphicsAPI.hpp"
#include IncludeDeferredBufferAPI

namespace Engine5
{
    class RendererCommon;
    class Color;

    class DeferredBufferCommon : public DeferredBufferAPI
    {
    public:
        DeferredBufferCommon();
        ~DeferredBufferCommon();

        bool Initialize(RendererCommon* renderer, U32 texture_width, U32 texture_height);
        void Render() const;
        void Shutdown();

        bool OnResize(U32 texture_width, U32 texture_height);

        void SetRenderTargets() const;
        void ClearRenderTargets(const Color& color);
    private:
    };

    
}

#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../../Core/Utility/CoreDef.hpp"
#include "../API/GraphicsAPI.hpp"
#include IncludeDeferredBufferAPI

namespace Engine5
{
    class Color;

    class DeferredBufferCommon : DeferredBufferAPI
    {
    public:
        DeferredBufferCommon();
        ~DeferredBufferCommon();

        bool Initialize(int texture_width, int texture_height, Real screen_depth, Real screen_near);
        void Shutdown();

        void SetRenderTargets() const;
        void ClearRenderTargets(const Color& color);

    private:
        U32  m_texture_width  = 512;
        U32  m_texture_height = 512;
        Real m_screen_depth   = 1.0f;
        Real m_screen_near    = 0.0f;
    };
}

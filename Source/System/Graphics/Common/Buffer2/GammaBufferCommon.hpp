#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeGammaBufferAPI
#include "../DataType/TopologyDef.hpp"
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"
#include "../DataType/BufferBindType.hpp"

namespace Engine5
{
    class RendererCommon;

    class GammaBufferCommon : public GammaBufferAPI
    {
    public:
        GammaBufferCommon();
        ~GammaBufferCommon();

        bool Init(RendererCommon* renderer, eBindingStage type);
        void Update(Real gamma = 2.2f) const;
        void Bind(U32 slot) const;
        void Shutdown();
    private:
        eBindingStage m_binding_type = eBindingStage::PixelShader;
    };
}

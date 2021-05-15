#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeLightBufferAPI
#include "../DataType/TopologyDef.hpp"
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"
#include "../DataType/BufferBindType.hpp"

namespace Engine5
{
    class RendererCommon;

    class LightBufferCommon : public LightBufferAPI
    {
    public:
        LightBufferCommon();
        ~LightBufferCommon();

        bool Init(RendererCommon* renderer, eBindingStage type);
        //void Update(const LightData& light_data) const;
        void Bind(U32 slot) const;
        void Shutdown();
    private:
        eBindingStage m_binding_type = eBindingStage::PixelShader;
    };
}

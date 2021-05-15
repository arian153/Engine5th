#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeConstantBufferAPI

#include "../../../Core/Utility/CoreDef.hpp"
#include "../DataType/BufferBindType.hpp"

namespace Engine5
{
    class Color;
    class RendererCommon;

    class ConstantBufferCommon : public ConstantBufferAPI
    {
    public:
        ConstantBufferCommon();
        ~ConstantBufferCommon();

        bool Init(RendererCommon* renderer, eBindingStage type, size_t buffer_size, U32 slot);
        void Update(void* data) const;
        void Bind() const;
        void Shutdown();
    private:
        U32           m_slot         = 0;
        eBindingStage m_stage = eBindingStage::PixelShader;
    };
}

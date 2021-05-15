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

        bool Init(RendererCommon* renderer, eBindingStage type, size_t buffer_size);
        void Update(void* data) const;
        void Bind(U32 slot) const;
        void Shutdown();
    private:
        eBindingStage m_binding_type = eBindingStage::PixelShader;
    };

    
}

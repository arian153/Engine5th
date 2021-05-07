#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeColorBufferAPI
#include "../DataType/TopologyDef.hpp"
#include <vector>


#include "../../../Core/Utility/CoreDef.hpp"
#include "../DataType/BufferBindType.hpp"

namespace Engine5
{
    class Color;
    class RendererCommon;

    class ColorBufferCommon : public ColorBufferAPI
    {
    public:
        ColorBufferCommon();
        ~ColorBufferCommon();

        bool Init(RendererCommon* renderer, eBufferBindType type);
        void Update(const Color& color) const;
        void Bind(U32 slot) const;
        void Shutdown();
    private:
        eBufferBindType m_binding_type = eBufferBindType::PixelShader;
    };

    
}

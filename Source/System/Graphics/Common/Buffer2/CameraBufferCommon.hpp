#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeCameraBufferAPI
#include "../DataType/TopologyDef.hpp"
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"
#include "../DataType/BufferBindType.hpp"

namespace Engine5
{
    class Camera;
    class RendererCommon;

    class CameraBufferCommon : public CameraBufferAPI
    {
    public:
        CameraBufferCommon();
        ~CameraBufferCommon();

        bool Init(RendererCommon* renderer, eBindingStage type);
        void Update(Camera* camera) const;
        void Bind(U32 slot) const;
        void Shutdown();
    private:
        eBindingStage m_binding_type = eBindingStage::VertexShader;
    };

  
}

#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeMatrixBufferAPI
#include "../DataType/TopologyDef.hpp"
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"
#include "../DataType/BufferBindType.hpp"

namespace Engine5
{
    class MatrixData;
    class RendererCommon;

    class MatrixBufferCommon : public MatrixBufferAPI
    {
    public:
        MatrixBufferCommon();
        ~MatrixBufferCommon();

        bool Init(RendererCommon* renderer, eBufferBindType type);
        void Update(const MatrixData& matrix_data) const;
        void Bind(U32 slot) const;
        void Shutdown();
    private:
        eBufferBindType m_binding_type = eBufferBindType::VertexShader;
    };

   
}

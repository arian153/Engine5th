#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeShaderProgramAPI

#include "../../../Math/Utility/MathDef.hpp"
#include "../../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class RendererCommon;
    class VertexLayoutCommon;

    class ShaderProgramCommon : public ShaderProgramAPI
    {
    public:
        ShaderProgramCommon();
        ~ShaderProgramCommon();

        bool Init(RendererCommon* renderer);
        void Shutdown();
        void Bind();

        void SetVertexLayout(VertexLayoutCommon* layout);

    private:
        VertexLayoutCommon* m_vertex_layout = nullptr;
    };

    
}

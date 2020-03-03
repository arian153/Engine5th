#pragma once

#include "../Renderer/APIMacro.hpp"
#include IncludeColorShaderAPI
#include <string>
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class Matrix44;

    class ColorShaderCommon : public ColorShaderAPI
    {
    public:
        ColorShaderCommon();
        ~ColorShaderCommon();

        void SetVertexShaderPath(const std::string& path);
        void SetPixelShaderPath(const std::string& path);

        bool Initialize();
        void Render(U32 indices_count, const Matrix44& world, const Matrix44& view, const Matrix44& proj) const;
        void Shutdown();

    private:
        std::string m_vertex_shader_path;
        std::string m_pixel_shader_path;
    };
}

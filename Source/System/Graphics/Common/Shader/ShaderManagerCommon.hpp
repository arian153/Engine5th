#pragma once
//#include <unordered_map>

#include <unordered_map>

#include "../../GraphicsAPI.hpp"
#include "../../../Core/Utility/CoreDef.hpp"

#include IncludeShaderManagerAPI
#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class TextureShaderCommon;
    class ConstantBufferCommon;
    class VertexLayoutCommon;
    class ShaderProgramCommon;
    class DirectionalLight;
    class Camera;
    class TextureCommon;
    class MatrixData;
    class ResourceManager;
    class RendererCommon;
    class Color;
    class TextureArrayCommon;

    class ShaderManagerCommon : public ShaderManagerAPI
    {
    public:
        ShaderManagerCommon();
        ~ShaderManagerCommon();
        void Initialize(RendererCommon* renderer, ResourceManager* resource_manager);
        void Shutdown();

        void AddShader(const std::wstring& name, VertexLayoutCommon* layout);

        void Bind(const std::string& name);
        void AddBuffer(const std::string& name, ConstantBufferCommon* constant_buffer);

        ShaderProgramCommon* GetShader(const std::string& name) const;

        //temp
    private:
        //shader tables
        std::unordered_map<std::string, ShaderProgramCommon*> m_shader_table;

        //vertex layouts
        VertexLayoutCommon* m_color_vertex_layout              = nullptr;
        VertexLayoutCommon* m_color_instancing_vertex_layout   = nullptr;
        VertexLayoutCommon* m_texture_vertex_layout            = nullptr;
        VertexLayoutCommon* m_texture_instancing_vertex_layout = nullptr;
        VertexLayoutCommon* m_general_vertex_layout            = nullptr;
        VertexLayoutCommon* m_general_instancing_vertex_layout = nullptr;

        //others
        RendererCommon*  m_renderer         = nullptr;
        ResourceManager* m_resource_manager = nullptr;
    };
}

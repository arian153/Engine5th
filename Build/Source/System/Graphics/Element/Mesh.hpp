#pragma once
#include "../../Math/Algebra/Matrix44.hpp"
#include "../Shader/ShaderType.hpp"
#include "../../Core/Utility/CoreDef.hpp"
#include "../DataType/Color.hpp"

namespace Engine5
{
    class Transform;
    class TextureCommon;
    class ShaderManager;
    class RendererCommon;
    class MeshData;
    class MeshBufferCommon;

    class Mesh
    {
    public:
        Mesh();
        ~Mesh();
        void Initialize();
        void Shutdown();

        void RenderBuffer() const;
        void RenderColorBuffer() const;
        void BuildBuffer();
        void SetMeshData(MeshData* mesh_data);
        void SetTexture(TextureCommon* texture);
        void SetTransform(Transform* transform);
        void SetShaderType(eShaderType type);
        void SetColor(const Color& color);
        void SetRenderer(RendererCommon* renderer);

        Matrix44       GetModelMatrix() const;
        TextureCommon* GetTexture() const;
        eShaderType    GetShaderType() const;
        U32            GetIndexCount() const;
        Color          GetColor() const;

    private:
        friend class MeshComponent;

    private:
        ShaderManager*  m_shader_manager = nullptr;
        RendererCommon* m_renderer       = nullptr;
        MeshBufferCommon*   m_buffer         = nullptr;
        MeshData*       m_mesh_data      = nullptr;
        TextureCommon*  m_texture        = nullptr;
        Transform*      m_transform      = nullptr;
        MeshComponent*  m_component      = nullptr;
        Color           m_color;
        eShaderType     m_type = eShaderType::Invalid;
    };
}

#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../../Math/Algebra/Matrix44.hpp"

namespace Engine5
{
    class Transform;
    class TextureCommon;
    class ShaderManager;
    class RendererCommon;
    class MeshData;
    class BufferCommon;

    class Mesh
    {
    public:
        Mesh();
        ~Mesh();
        void Initialize();
        void Shutdown();

        void RenderBuffer() const;
        void BuildBuffer();
        void SetMeshData(MeshData* mesh_data);
        void SetTexture(TextureCommon* texture);
        void SetTransform(Transform* transform);

        
        Matrix44       GetModelMatrix() const;
        TextureCommon* GetTexture() const;

    private:
        ShaderManager*  m_shader_manager = nullptr;
        RendererCommon* m_renderer       = nullptr;
        BufferCommon*   m_buffer         = nullptr;
        MeshData*       m_mesh_data      = nullptr;
        TextureCommon*  m_texture        = nullptr;
        Transform*      m_transform      = nullptr;
    };
}

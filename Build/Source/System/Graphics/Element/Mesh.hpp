#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../../Math/Algebra/Matrix44.hpp"

namespace Engine5
{
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
        void Render() const;
        void Shutdown();

        void BuildBuffer();
        void SetMeshData(MeshData* mesh_data);
        void SetTexture(TextureCommon* texture);

        Matrix44 GetModelMatrix();
        TextureCommon* GetTexture();

    private:
        ShaderManager*  m_shader_manager = nullptr;
        RendererCommon* m_renderer       = nullptr;
        BufferCommon*   m_buffer         = nullptr;
        MeshData*       m_mesh_data      = nullptr;
        TextureCommon*  m_texture        = nullptr;
    };
}

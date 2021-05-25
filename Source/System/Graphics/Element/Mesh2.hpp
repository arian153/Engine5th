#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../Common/Texture/TextureArrayCommon.hpp"

namespace Engine5
{
    struct InstanceBufferData;
    class InstanceBufferCommon;
    class IndexBufferCommon;
    class VertexBufferCommon;
    class Transform;
    class MeshData;
    class RendererCommon;
    class ShaderManagerCommon;

    class Mesh2
    {
    public:
        Mesh2();
        ~Mesh2();

        void Initialize();
        void Shutdown();

        void Render() const;
        void BuildBuffer();

        void AddInstance(const InstanceBufferData& data);

        void SetModelData(MeshData* data);
        void AddTexture(TextureCommon* texture);
        void ClearTexture();
        void RemoveTexture(TextureCommon* texture);
        void SetRenderer(RendererCommon* renderer);
        void ResizeInstanceBuffer(U32 count);

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        RendererCommon*      m_renderer       = nullptr;
        MeshData*            m_mesh_data      = nullptr;
        TextureArrayCommon   m_diffuse_maps;
        TextureArrayCommon   m_specular_maps;
        TextureArrayCommon   m_normal_maps;

        VertexBufferCommon*   m_vertex_buffer   = nullptr;
        IndexBufferCommon*    m_index_buffer    = nullptr;
        InstanceBufferCommon* m_instance_buffer = nullptr;
        U32                   m_stride          = 0;
        U32                   m_max_count       = 0;
        U32                   m_instance_count  = 0;

        std::vector<InstanceBufferData> m_instances;
    };
}

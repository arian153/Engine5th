#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../Common/Texture/TextureArrayCommon.hpp"

namespace Engine5
{
    class Transform;
    class MeshData;
    class RendererCommon;
    class ShaderManagerCommon;

    class Mesh2
    {
    public:
        Mesh2();
        ~Mesh2();

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        RendererCommon*      m_renderer       = nullptr;
        MeshData*            m_mesh_data      = nullptr;
        TextureArrayCommon   m_texture_array;
        Transform*           m_transform = nullptr;
        U32                  m_stride    = 0;
    };
}

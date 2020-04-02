#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
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

        void Build();
        void SetMeshData(MeshData* mesh_data);

    private:
        RendererCommon* m_renderer  = nullptr;
        BufferCommon*   m_buffer    = nullptr;
        MeshData*       m_mesh_data = nullptr;
    };
}

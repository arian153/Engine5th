#pragma once
#include <directxmath.h>
#include <d3d11.h>
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    struct VertexType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };

    class Mesh
    {
    public:
        Mesh();
        ~Mesh();
        void Initialize();
        void Update(Real dt) const;
        void Shutdown();

        void SetDevice(ID3D11Device* device, ID3D11DeviceContext* context);
        int  GetIndexCount() const;
    private:
        void RenderBuffers() const;

    private:
        ID3D11Device*        m_device         = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Buffer*        m_vertex_buffer  = nullptr;
        ID3D11Buffer*        m_index_buffer   = nullptr;;
        int                  m_vertex_count   = 0;
        int                  m_index_count    = 0;
    };
}

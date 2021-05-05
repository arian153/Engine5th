#include "VertexBufferDX11.hpp"

#include "../../Common/Buffer2/VertexBufferCommon.hpp"

namespace Engine5
{
    VertexBufferDX11::VertexBufferDX11()
    {
    }

    VertexBufferDX11::~VertexBufferDX11()
    {
    }

    VertexBufferCommon::VertexBufferCommon()
    {
    }

    VertexBufferCommon::~VertexBufferCommon()
    {
    }

    void VertexBufferCommon::SetPrimitiveTopology(eTopologyType type)
    {
        switch (type)
        {
        case eTopologyType::PointList:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case eTopologyType::LineList:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case eTopologyType::TriangleList:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        default:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        }
    }
}

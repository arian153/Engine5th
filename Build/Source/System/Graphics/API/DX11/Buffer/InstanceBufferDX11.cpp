#include "InstanceBufferDX11.hpp"
#include "../../../Buffer/InstanceBufferCommon.hpp"

namespace Engine5
{
    InstanceBufferDX11::InstanceBufferDX11()
    {
    }

    InstanceBufferDX11::~InstanceBufferDX11()
    {
    }

    InstanceBufferCommon::InstanceBufferCommon()
    {
    }

    InstanceBufferCommon::~InstanceBufferCommon()
    {
    }

    void InstanceBufferCommon::Render(U32 stride, U32 offset) const
    {
    }

    void InstanceBufferCommon::Shutdown()
    {
    }

    bool InstanceBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, const std::vector<InstanceDataCommon>& instances)
    {
    }

    bool InstanceBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, const std::vector<InstanceDataCommon>& instances)
    {
    }

    bool InstanceBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, const std::vector<InstanceDataCommon>& instances)
    {
    }
}

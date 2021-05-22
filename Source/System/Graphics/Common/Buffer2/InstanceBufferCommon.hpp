#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeInstanceBufferAPI
#include "../../../Core/Utility/CoreDef.hpp"
#include <vector>
#include "../Vertex/NormalVertexCommon.hpp"
#include "../Vertex/ColorVertexCommon.hpp"
#include "../Vertex/InstanceDataCommon.hpp"
#include "../Vertex/TextureVertexCommon.hpp"

namespace Engine5
{
    class RendererCommon;

    class InstanceBufferCommon : public InstanceBufferAPI
    {
    public:
        InstanceBufferCommon();
        ~InstanceBufferCommon();

        void Render(U32 stride, U32 offset) const;
        void Shutdown();

        bool BuildBuffer(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, const std::vector<U32>& indices);


        void Init(RendererCommon* renderer, const std::vector<InstanceData>& instances);
        void* GetInstanceBuffer() const;

        void BuildInstanceBuffer(const std::vector<InstanceDataCommon>& instances);
        void UpdateInstanceBuffer(const std::vector<InstanceDataCommon>& instances) const;
    private:
    };
}

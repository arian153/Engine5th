#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeVertexBufferAPI
#include "../../DataType/TopologyDef.hpp"
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"
#include "../Vertex/ColorVertexCommon.hpp"
#include "../Vertex/GeneralVertexCommon.hpp"
#include "../Vertex/NormalVertexCommon.hpp"
#include "../Vertex/TextureVertexCommon.hpp"

namespace Engine5
{
    class InstanceBufferCommon;
    class RendererCommon;

    class VertexBufferCommon : public VertexBufferAPI
    {
    public:
        VertexBufferCommon();
        ~VertexBufferCommon();

        bool Init(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, bool is_dynamic = false);
        bool Init(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, bool is_dynamic = false);
        bool Init(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, bool is_dynamic = false);
        bool Init(RendererCommon* renderer, const std::vector<GeneralVertexCommon>& vertices, bool is_dynamic = false);

        void Update(const std::vector<ColorVertexCommon>& vertices) const;
        void Update(const std::vector<TextureVertexCommon>& vertices) const;
        void Update(const std::vector<NormalVertexCommon>& vertices) const;
        void Update(const std::vector<GeneralVertexCommon>& vertices) const;

        bool Init(RendererCommon* renderer, void* data, size_t vertex_size, size_t vertex_count, bool is_dynamic = false);
        void Update(void* data, size_t vertex_size, size_t vertex_count) const;
        void Bind(U32 stride, U32 offset) const;
        void Bind(U32 stride, U32 offset, InstanceBufferCommon* instance_buffer) const;
        void Shutdown();
        void SetPrimitiveTopology(eTopologyType type);

    private:
        bool m_b_dynamic = false;
    };
}

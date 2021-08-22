#pragma once
#include <vector>

#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class TextureCommon;
    class RendererCommon;
    class VertexBufferCommon;
    class IndexBufferCommon;

    class SkyBox
    {
    public:
        SkyBox();
        ~SkyBox();

        void Initialize();
        void Update(Real dt);
        void Shutdown();


        void Bind() const;
        void Draw() const;
    private:
        RendererCommon*     m_renderer      = nullptr;
        VertexBufferCommon* m_vertex_buffer = nullptr;
        IndexBufferCommon*  m_index_buffer  = nullptr;
        TextureCommon*      m_texture       = nullptr;

        std::vector<U32>     m_indices;
        std::vector<Vector3> m_vertices;
    };
}

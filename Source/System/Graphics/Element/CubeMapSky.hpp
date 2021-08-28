#pragma once
#include <vector>

#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Algebra/Matrix44.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class SkyComponent;
    class Camera;
    class TextureCommon;
    class RendererCommon;
    class VertexBufferCommon;
    class IndexBufferCommon;

    class CubeMapSky
    {
    public:
        CubeMapSky();
        ~CubeMapSky();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Bind() const;
        void Draw() const;

        void GenerateSkySphere();
        void SetCamera(Camera* camera);

        Matrix44 GetModelMatrix() const;

    private:
        friend class SkyComponent;

    private:
        SkyComponent* m_component     = nullptr;
        RendererCommon*          m_renderer      = nullptr;
        VertexBufferCommon*      m_vertex_buffer = nullptr;
        IndexBufferCommon*       m_index_buffer  = nullptr;
        TextureCommon*           m_texture       = nullptr;
        Camera*                  m_camera        = nullptr;

        Transform m_transform;
        float     m_radius = 5000.0f;

        std::vector<U32>     m_indices;
        std::vector<Vector3> m_vertices;
    };
}

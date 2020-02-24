#pragma once
#include "../../Math/Math.hpp"
#include "Color.hpp"
#include <directxmath.h>
#include <d3d11.h>
#include <vector>

namespace Engine5
{
    class RendererCommon;
    class Camera;
    class ColorShader;

    enum class eRenderingMode
    {
        Dot
      , Line
      , Face
    };

    class PrimitiveRenderer
    {
    public:
        explicit PrimitiveRenderer(RendererCommon* dx11_api);
        ~PrimitiveRenderer();
        void DrawPrimitive(Primitive* primitive, eRenderingMode mode, Color color = Color());
        void DrawPrimitive(const Primitive& primitive, eRenderingMode mode, Color color = Color());

        //Draw Direct Primitives
        void DrawPoint(const Vector3& point, Color color = Color());
        void DrawSegment(const Vector3& start, const Vector3& end, Color color = Color());
        void DrawTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, eRenderingMode mode, Color color = Color());
        void DrawTetrahedron(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, eRenderingMode mode, Color color = Color());

        void Initialize(ColorShader* color_shader);
        void Update(Real dt);
        void Shutdown();

        void UpdatePrimitiveRendererCamera();
        void Clear();

        void UpdateProjectionMatrix();
        void SetRendererCameraPosition(const Vector3& pos);
        void SetRendererCameraRotation(const Quaternion& rot);
        //void SyncWithCamera(Camera* camera);

        void   PushVertex(const Vector3& pos, eRenderingMode mode, const Color& color = Color());
        void   PushIndex(I32 index, eRenderingMode mode);
        void   PushLineIndices(I32 a, I32 b);
        void   PushFaceIndices(I32 a, I32 b, I32 c);
        void   ReserveVertices(size_t adding_count, eRenderingMode mode);
        void   ReserveIndices(size_t adding_count, eRenderingMode mode);
        size_t VerticesSize(eRenderingMode mode) const;
        size_t IndicesSize(eRenderingMode mode) const;

    private:
        void BuildDotBuffer();
        void UpdateDotBuffer(Real dt) const;

        void BuildLineBuffer();
        void UpdateLineBuffer(Real dt) const;

        void BuildTriangleBuffer();
        void UpdateTriangleBuffer(Real dt) const;

    private:

        class ColorVertex
        {
        public:
            ColorVertex()
            {
                pos.x = 0.0f;
                pos.y = 0.0f;
                pos.z = 0.0f;
                clr.w = 1.0f;
                clr.x = 1.0f;
                clr.y = 1.0f;
                clr.z = 1.0f;
            }

            ColorVertex(const Vector3& position, const Color& color)
            {
                pos.x = position.x;
                pos.y = position.y;
                pos.z = position.z;
                clr   = color.ToXMFloat4();
            }

        public:
            DirectX::XMFLOAT3 pos;
            DirectX::XMFLOAT4 clr;
        };

    private:
        Vector3              m_position = Vector3(0.0f, 0.0f, -10.0f);
        Quaternion           m_rotation = Quaternion();
        DirectX::XMMATRIX    m_view_matrix;
        DirectX::XMMATRIX    m_world_matrix;
        DirectX::XMMATRIX    m_proj_matrix;
        ColorShader*         m_color_shader   = nullptr;
        RendererCommon*      m_renderer       = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;

        std::vector<ColorVertex> m_dot_vertices;
        std::vector<I32>         m_dot_indices;
        ID3D11Buffer*            m_dot_vertex_buffer = nullptr;
        ID3D11Buffer*            m_dot_index_buffer  = nullptr;

        std::vector<ColorVertex> m_line_vertices;
        std::vector<I32>         m_line_indices;
        ID3D11Buffer*            m_line_vertex_buffer = nullptr;
        ID3D11Buffer*            m_line_index_buffer  = nullptr;

        std::vector<ColorVertex> m_face_vertices;
        std::vector<I32>         m_face_indices;
        ID3D11Buffer*            m_face_vertex_buffer = nullptr;
        ID3D11Buffer*            m_face_index_buffer  = nullptr;

    public:
        const int CIRCULAR_VERTICES_COUNT    = 100;
        const int SPHERICAL_SLICE_COUNT      = 14;
        const int SPHERICAL_STACK_COUNT      = 15;
        const int SPHERICAL_VERTICES_COUNT   = 212; //(stack_count - 1) * (slice_count + 1) + 2;
        const int CYLINDRICAL_SLICE_COUNT    = 20;
        const int CYLINDRICAL_STACK_COUNT    = 1;
        const int CYLINDRICAL_VERTICES_COUNT = 42; //(stack_count + 1) * (slice_count + 1)
    };
}

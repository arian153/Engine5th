#pragma once
#include "../../Math/Math.hpp"
#include <vector>
#include "../DataType/Color.hpp"
#include "../DataType/MatrixData.hpp"
#include "../Vertex/ColorVertex.hpp"

namespace Engine5
{
    class MatrixManager;
    class MeshBufferCommon;
    class RendererCommon;
    class Camera;
    class ColorShaderCommon;

    enum class eRenderingMode
    {
        Dot
      , Line
      , Face
    };

    class PrimitiveRenderer
    {
    public:
        explicit PrimitiveRenderer(RendererCommon* renderer);
        ~PrimitiveRenderer();
        void DrawPrimitive(Primitive* primitive, eRenderingMode mode, Color color = Color());
        void DrawPrimitive(const Primitive& primitive, eRenderingMode mode, Color color = Color());

        //Draw Direct Primitives
        void DrawPoint(const Vector3& point, Color color = Color());
        void DrawSegment(const Vector3& start, const Vector3& end, Color color = Color());
        void DrawTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, eRenderingMode mode, Color color = Color());
        void DrawTetrahedron(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, eRenderingMode mode, Color color = Color());
        void DrawBox(const Vector3& position, const Quaternion& orientation, const Vector3& scale, eRenderingMode mode, Color color = Color());

        void Initialize(ColorShaderCommon* color_shader);
        void Update();
        void Shutdown();
        void Clear();

        void UpdateViewMatrix(const Matrix44& view_matrix);
        void UpdateProjectionMatrix(const Matrix44& projection_matrix);

        void   PushVertex(const Vector3& pos, eRenderingMode mode, const Color& color = Color());
        void   PushIndex(I32 index, eRenderingMode mode);
        void   PushLineIndices(I32 a, I32 b);
        void   PushFaceIndices(I32 a, I32 b, I32 c);
        void   ReserveVertices(size_t adding_count, eRenderingMode mode);
        void   ReserveIndices(size_t adding_count, eRenderingMode mode);
        size_t VerticesSize(eRenderingMode mode) const;
        size_t IndicesSize(eRenderingMode mode) const;
    private:
        MatrixData         m_mvp_data;
        ColorShaderCommon* m_color_shader = nullptr;
        RendererCommon*    m_renderer     = nullptr;

        std::vector<ColorVertex> m_dot_vertices;
        std::vector<U32>         m_dot_indices;
        MeshBufferCommon*            m_dot_buffer = nullptr;

        std::vector<ColorVertex> m_line_vertices;
        std::vector<U32>         m_line_indices;
        MeshBufferCommon*            m_line_buffer = nullptr;

        std::vector<ColorVertex> m_face_vertices;
        std::vector<U32>         m_face_indices;
        MeshBufferCommon*            m_face_buffer = nullptr;

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

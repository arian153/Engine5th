#include "PrimitiveRenderer.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "MatrixManager.hpp"
#include "../../Math/Curve/Curve.hpp"
#include "../DataType/MatrixData.hpp"
#include "../DataType/TopologyDef.hpp"
#include "../Common/Buffer/MeshBufferCommon.hpp"
#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../Common/Shader/ShaderProgramCommon.hpp"

namespace Engine5
{
    PrimitiveRenderer::PrimitiveRenderer(RendererCommon* renderer)
        : m_renderer(renderer)
    {
    }

    PrimitiveRenderer::~PrimitiveRenderer()
    {
    }

    void PrimitiveRenderer::DrawPrimitive(Primitive* primitive, eRenderingMode mode, Color color)
    {
        primitive->DrawPrimitive(this, mode, color);
    }

    void PrimitiveRenderer::DrawPrimitive(const Primitive& primitive, eRenderingMode mode, Color color)
    {
        primitive.DrawPrimitive(this, mode, color);
    }

    void PrimitiveRenderer::DrawPoint(const Vector3& point, Color color)
    {
        I32 index = (I32)m_dot_vertices.size();
        I32 count = index + 1;
        m_dot_vertices.reserve(static_cast<size_t>(count));
        m_dot_vertices.emplace_back(point, color);
        m_dot_indices.push_back(index);
    }

    void PrimitiveRenderer::DrawSegment(const Vector3& start, const Vector3& end, Color color)
    {
        I32 index = (I32)m_line_vertices.size();
        I32 count = index + 2;
        m_line_vertices.reserve(static_cast<size_t>(count));
        m_line_vertices.emplace_back(start, color);
        m_line_vertices.emplace_back(end, color);
        PushLineIndices(index, index + 1);
    }

    void PrimitiveRenderer::DrawTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, eRenderingMode mode, Color color)
    {
        I32 index = static_cast<I32>(VerticesSize(mode));
        I32 count = 3;
        ReserveVertices(count, mode);
        PushVertex(p0, mode, color);
        PushVertex(p1, mode, color);
        PushVertex(p2, mode, color);
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (int i = 0; i < count; ++i)
            {
                I32 j = i + 1 < count ? i + 1 : 0;
                PushLineIndices(index + i, index + j);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            PushFaceIndices(index + 0, index + 1, index + 2);
            PushFaceIndices(index + 0, index + 2, index + 1);
        }
    }

    void PrimitiveRenderer::DrawRectangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, eRenderingMode mode, Color color)
    {
        I32 index = static_cast<I32>(VerticesSize(mode));
        I32 count = 4;
        ReserveVertices(count, mode);
        PushVertex(p0, mode, color);
        PushVertex(p1, mode, color);
        PushVertex(p2, mode, color);
        PushVertex(p3, mode, color);

        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (int i = 0; i < count; ++i)
            {
                I32 j = i + 1 < count ? i + 1 : 0;
                PushLineIndices(index + i, index + j);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            PushFaceIndices(index + 0, index + 1, index + 2);
            PushFaceIndices(index + 0, index + 2, index + 3);
            PushFaceIndices(index + 0, index + 2, index + 1);
            PushFaceIndices(index + 0, index + 3, index + 2);
        }
    }

    void PrimitiveRenderer::DrawTetrahedron(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, eRenderingMode mode, Color color)
    {
        I32 index = static_cast<I32>(VerticesSize(mode));
        I32 count = 4;
        ReserveVertices(count, mode);
        PushVertex(p0, mode, color);
        PushVertex(p1, mode, color);
        PushVertex(p2, mode, color);
        PushVertex(p3, mode, color);
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            PushLineIndices(index, index + 1);
            PushLineIndices(index, index + 2);
            PushLineIndices(index, index + 3);
            PushLineIndices(index + 1, index + 2);
            PushLineIndices(index + 2, index + 3);
            PushLineIndices(index + 3, index + 1);
        }
        else if (mode == eRenderingMode::Face)
        {
            PushFaceIndices(index, index + 1, index + 2);
            PushFaceIndices(index, index + 2, index + 3);
            PushFaceIndices(index, index + 3, index + 1);
            PushFaceIndices(index + 1, index + 2, index + 3);
        }
    }

    void PrimitiveRenderer::DrawBox(const Vector3& position, const Quaternion& orientation, const Vector3& scale, eRenderingMode mode, Color color)
    {
        I32 index = static_cast<I32>(VerticesSize(mode));
        I32 count = 8;
        ReserveVertices(count, mode);
        Vector3 dim = scale.Half();
        Vector3 vertices[8];
        vertices[0].Set(+dim.x, +dim.y, +dim.z);
        vertices[1].Set(+dim.x, +dim.y, -dim.z);
        vertices[2].Set(+dim.x, -dim.y, +dim.z);
        vertices[3].Set(+dim.x, -dim.y, -dim.z);
        vertices[4].Set(-dim.x, +dim.y, +dim.z);
        vertices[5].Set(-dim.x, +dim.y, -dim.z);
        vertices[6].Set(-dim.x, -dim.y, +dim.z);
        vertices[7].Set(-dim.x, -dim.y, -dim.z);
        for (size_t i = 0; i < 8; ++i)
        {
            vertices[i] = orientation.Rotate(vertices[i]);
            vertices[i] += position;
            PushVertex(vertices[i], mode, color);
        }
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            //front
            PushLineIndices(index + 0, index + 2);
            PushLineIndices(index + 2, index + 6);
            PushLineIndices(index + 6, index + 4);
            PushLineIndices(index + 4, index + 0);
            //back
            PushLineIndices(index + 1, index + 3);
            PushLineIndices(index + 3, index + 7);
            PushLineIndices(index + 7, index + 5);
            PushLineIndices(index + 5, index + 1);
            //side
            PushLineIndices(index + 0, index + 1);
            PushLineIndices(index + 2, index + 3);
            PushLineIndices(index + 6, index + 7);
            PushLineIndices(index + 4, index + 5);
        }
        else if (mode == eRenderingMode::Face)
        {
            //front
            PushFaceIndices(index + 0, index + 2, index + 4);
            PushFaceIndices(index + 2, index + 6, index + 4);
            //back
            PushFaceIndices(index + 1, index + 3, index + 5);
            PushFaceIndices(index + 3, index + 7, index + 5);
            //right
            PushFaceIndices(index + 0, index + 1, index + 3);
            PushFaceIndices(index + 2, index + 3, index + 0);
            //left
            PushFaceIndices(index + 6, index + 7, index + 5);
            PushFaceIndices(index + 4, index + 5, index + 6);
            //top
            PushFaceIndices(index + 1, index + 0, index + 4);
            PushFaceIndices(index + 1, index + 4, index + 5);
            //bottom
            PushFaceIndices(index + 3, index + 2, index + 6);
            PushFaceIndices(index + 3, index + 6, index + 7);
        }
    }

    void PrimitiveRenderer::DrawRay(const Ray& ray, Color color)
    {
        I32     index = (I32)m_line_vertices.size();
        I32     count = index + 2;
        Vector3 line_start, line_end;
        m_frustum.IntersectRay(ray, line_start, line_end);
        m_line_vertices.reserve(static_cast<size_t>(count));
        m_line_vertices.emplace_back(line_start, color);
        m_line_vertices.emplace_back(line_end, color);
        PushLineIndices(index, index + 1);
    }

    void PrimitiveRenderer::DrawArrow(const Vector3& start, const Vector3& end, Color color)
    {
        if (end.DistanceSquaredTo(start) > Math::EPSILON)
        {
            Real       scale = start.DistanceTo(end);
            Quaternion rotation(Vector3(0.0f, 1.0f, 0.0f), (end - start).Normalize());
            DrawPrimitive(Cone(end, rotation, scale * 0.1f, scale * 0.2f), eRenderingMode::Face, color);
            DrawSegment(start, end, color);
        }
    }

    void PrimitiveRenderer::DrawCurveLine(const Curve& curve, Color color)
    {
        size_t curve_size = curve.points.size() - 1;
        size_t index      = m_line_vertices.size();

        m_line_vertices.reserve(index + 2 * curve_size);
        m_line_indices.reserve(m_line_indices.size() + 2 * curve_size);

        for (size_t i = 0; i < curve_size; i++)
        {
            m_line_vertices.emplace_back(curve.points[i], color);
            PushLineIndices((I32)(index + i), (I32)(index + i + 1));
        }
        m_line_vertices.emplace_back(curve.points[curve_size], color);
    }

    void PrimitiveRenderer::DrawDashedLineSegment(const Vector3& start, const Vector3& end, Real length, Color color)
    {
        Real    distance = start.DistanceTo(end);
        Vector3 dir      = (end - start).Unit();
        int     count    = static_cast<int>(distance / length);

        for (int i = 0; i < count; i += 2)
        {
            Vector3 a = start + static_cast<Real>(i) * length * dir;
            Vector3 b = start + static_cast<Real>(i + 1) * length * dir;
            DrawSegment(a, b, color);
        }
    }

    void PrimitiveRenderer::Initialize(ShaderProgramCommon* color_shader)
    {
        m_shader = color_shader;

        m_dot_vertex_buffer  = new VertexBufferCommon();
        m_line_vertex_buffer = new VertexBufferCommon();
        m_face_vertex_buffer = new VertexBufferCommon();

        m_dot_index_buffer  = new IndexBufferCommon();
        m_line_index_buffer = new IndexBufferCommon();
        m_face_index_buffer = new IndexBufferCommon();

        m_stride = sizeof(ColorVertexCommon);
    }

    void PrimitiveRenderer::Render() const
    {
    }

    void PrimitiveRenderer::PreRenderDot() const
    {
        if (m_dot_vertices.empty() == false)
        {
            //build buffer
            m_dot_index_buffer->Init(m_renderer, m_dot_indices);
            m_dot_vertex_buffer->Init(m_renderer, m_dot_vertices);
            m_dot_vertex_buffer->SetPrimitiveTopology(eTopologyType::PointList);
            //Bind Buffer
            m_dot_vertex_buffer->Bind(0);
            m_dot_index_buffer->Bind(0);
            m_shader->Bind();
        }
    }

    void PrimitiveRenderer::PreRenderLine() const
    {
        if (m_line_vertices.empty() == false)
        {
            //build buffer
            m_line_index_buffer->Init(m_renderer, m_line_indices);
            m_line_vertex_buffer->Init(m_renderer, m_line_vertices);
            m_line_vertex_buffer->SetPrimitiveTopology(eTopologyType::LineList);

            //Bind Buffer
            m_line_vertex_buffer->Bind(0);
            m_line_index_buffer->Bind(0);
            m_shader->Bind();
        }
    }

    void PrimitiveRenderer::PreRenderFace() const
    {
        if (m_face_vertices.empty() == false)
        {
            //build buffer
            m_face_index_buffer->Init(m_renderer, m_face_indices);
            m_face_vertex_buffer->Init(m_renderer, m_face_vertices);
            m_face_vertex_buffer->SetPrimitiveTopology(eTopologyType::TriangleList);
            //Bind Buffer
            m_face_vertex_buffer->Bind(0);
            m_face_index_buffer->Bind(0);
            m_shader->Bind();
        }
    }

    void PrimitiveRenderer::PostRenderDot() const
    {
        if (m_dot_vertices.empty() == false)
        {
            m_dot_index_buffer->Draw();
        }
    }

    void PrimitiveRenderer::PostRenderLine() const
    {
        if (m_line_vertices.empty() == false)
        {
            m_line_index_buffer->Draw();
        }
    }

    void PrimitiveRenderer::PostRenderFace() const
    {
        if (m_face_vertices.empty() == false)
        {
            m_face_index_buffer->Draw();
        }
    }

    void PrimitiveRenderer::Shutdown()
    {
        Clear();
        if (m_dot_vertex_buffer != nullptr)
        {
            m_dot_vertex_buffer->Shutdown();
            delete m_dot_vertex_buffer;
            m_dot_vertex_buffer = nullptr;
        }
        if (m_line_vertex_buffer != nullptr)
        {
            m_line_vertex_buffer->Shutdown();
            delete m_line_vertex_buffer;
            m_line_vertex_buffer = nullptr;
        }
        if (m_face_vertex_buffer != nullptr)
        {
            m_face_vertex_buffer->Shutdown();
            delete m_face_vertex_buffer;
            m_face_vertex_buffer = nullptr;
        }

        if (m_dot_index_buffer != nullptr)
        {
            m_dot_index_buffer->Shutdown();
            delete m_dot_index_buffer;
            m_dot_index_buffer = nullptr;
        }
        if (m_line_index_buffer != nullptr)
        {
            m_line_index_buffer->Shutdown();
            delete m_line_index_buffer;
            m_line_index_buffer = nullptr;
        }
        if (m_face_index_buffer != nullptr)
        {
            m_face_index_buffer->Shutdown();
            delete m_face_index_buffer;
            m_face_index_buffer = nullptr;
        }
    }

    void PrimitiveRenderer::Clear()
    {
        m_dot_vertices.clear();
        m_dot_indices.clear();
        m_line_vertices.clear();
        m_line_indices.clear();
        m_face_vertices.clear();
        m_face_indices.clear();
    }

    void PrimitiveRenderer::UpdateViewMatrix(const Matrix44& view_matrix)
    {
        m_mvp_data.view = view_matrix;
    }

    void PrimitiveRenderer::UpdateProjectionMatrix(const Matrix44& projection_matrix)
    {
        m_mvp_data.projection = projection_matrix;
    }

    void PrimitiveRenderer::UpdateFrustum(const Frustum& frustum)
    {
        m_frustum = frustum;
    }

    void PrimitiveRenderer::PushVertex(const Vector3& pos, eRenderingMode mode, const Color& color)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_vertices.emplace_back(pos, color);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_vertices.emplace_back(pos, color);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_vertices.emplace_back(pos, color);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::PushIndex(I32 index, eRenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_indices.push_back(index);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_indices.push_back(index);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_indices.push_back(index);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::PushLineIndices(I32 a, I32 b)
    {
        m_line_indices.push_back(a);
        m_line_indices.push_back(b);
    }

    void PrimitiveRenderer::PushFaceIndices(I32 a, I32 b, I32 c)
    {
        m_face_indices.push_back(a);
        m_face_indices.push_back(b);
        m_face_indices.push_back(c);
    }

    void PrimitiveRenderer::ReserveVertices(size_t adding_count, eRenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_vertices.reserve(m_dot_vertices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_vertices.reserve(m_line_vertices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_vertices.reserve(m_face_vertices.size() + adding_count);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::ReserveIndices(size_t adding_count, eRenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_indices.reserve(m_dot_indices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_indices.reserve(m_line_indices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_indices.reserve(m_face_indices.size() + adding_count);
            break;
        default:
            break;
        }
    }

    size_t PrimitiveRenderer::VerticesSize(eRenderingMode mode) const
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            return m_dot_vertices.size();
        case Engine5::eRenderingMode::Line:
            return m_line_vertices.size();
        case Engine5::eRenderingMode::Face:
            return m_face_vertices.size();
        default:
            break;
        }
        return 0;
    }

    size_t PrimitiveRenderer::IndicesSize(eRenderingMode mode) const
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            return m_dot_indices.size();
        case Engine5::eRenderingMode::Line:
            return m_line_indices.size();
        case Engine5::eRenderingMode::Face:
            return m_face_indices.size();
        default:
            break;
        }
        return 0;
    }
}

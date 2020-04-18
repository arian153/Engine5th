#include "PrimitiveRenderer.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "../Shader/Forward/ColorShaderCommon.hpp"
#include "MatrixManager.hpp"
#include "../DataType/MatrixData.hpp"
#include "../DataType/TopologyDef.hpp"
#include "../Buffer/MeshBufferCommon.hpp"

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
        Vector3 vertices[ 8 ];
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

    void PrimitiveRenderer::Initialize(ColorShaderCommon* color_shader)
    {
        m_color_shader = color_shader;
        m_dot_buffer   = new MeshBufferCommon();
        m_line_buffer  = new MeshBufferCommon();
        m_face_buffer  = new MeshBufferCommon();
    }

    void PrimitiveRenderer::Render()
    {
        if (m_dot_vertices.empty() == false)
        {
            m_dot_buffer->BuildBuffer(m_renderer, m_dot_vertices, m_dot_indices);
            m_dot_buffer->Render(sizeof(ColorVertexCommon), 0, eTopologyType::PointList);
            m_color_shader->Render(static_cast<U32>(m_dot_indices.size()), m_mvp_data);
        }
        if (m_line_vertices.empty() == false)
        {
            m_line_buffer->BuildBuffer(m_renderer, m_line_vertices, m_line_indices);
            m_line_buffer->Render(sizeof(ColorVertexCommon), 0, eTopologyType::LineList);
            m_color_shader->Render(static_cast<U32>(m_line_indices.size()), m_mvp_data);
        }
        if (m_face_vertices.empty() == false)
        {
            m_face_buffer->BuildBuffer(m_renderer, m_face_vertices, m_face_indices);
            m_face_buffer->Render(sizeof(ColorVertexCommon), 0);
            m_color_shader->Render(static_cast<U32>(m_face_indices.size()), m_mvp_data);
        }
        Clear();
    }

    void PrimitiveRenderer::Shutdown()
    {
        Clear();
        if (m_dot_buffer != nullptr)
        {
            m_dot_buffer->Shutdown();
            delete m_dot_buffer;
            m_dot_buffer = nullptr;
        }
        if (m_line_buffer != nullptr)
        {
            m_line_buffer->Shutdown();
            delete m_line_buffer;
            m_line_buffer = nullptr;
        }
        if (m_face_buffer != nullptr)
        {
            m_face_buffer->Shutdown();
            delete m_face_buffer;
            m_face_buffer = nullptr;
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

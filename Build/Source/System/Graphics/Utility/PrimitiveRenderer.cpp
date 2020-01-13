#include "PrimitiveRenderer.hpp"
#include "../Shader/ColorShader.hpp"

namespace Engine5
{
    PrimitiveRenderer::PrimitiveRenderer(DirectX3D11* dx11_api)
        : m_view_matrix(), m_world_matrix(), m_proj_matrix(), m_dx11_api(dx11_api)
    {
    }

    PrimitiveRenderer::~PrimitiveRenderer()
    {
    }

    void PrimitiveRenderer::DrawPrimitive(Primitive* primitive, RenderingMode mode, Color color)
    {
        primitive->DrawPrimitive(this, mode, color);
    }

    void PrimitiveRenderer::DrawPrimitive(const Primitive& primitive, RenderingMode mode, Color color)
    {
        primitive.DrawPrimitive(this, mode, color);
    }

    void PrimitiveRenderer::Initialize(ColorShader* color_shader)
    {
        m_color_shader = color_shader;
        m_position     = Vector3(0.0f, 0.0f, -60.0f);
        m_rotation     = Quaternion();
        UpdatePrimitiveRendererCamera();
        UpdateProjectionMatrix();
        m_device_context = m_dx11_api->GetDeviceContext();
        m_world_matrix = DirectX::XMMatrixIdentity();
    }

    void PrimitiveRenderer::Update(Real dt)
    {
        UpdatePrimitiveRendererCamera();
        //
        BuildDotBuffer();
        UpdateDotBuffer(dt);
        //line
        BuildLineBuffer();
        UpdateLineBuffer(dt);
        
        //face
        BuildTriangleBuffer();
        UpdateTriangleBuffer(dt);
        Clear();
    }

    void PrimitiveRenderer::Shutdown()
    {
        Clear();
        if (m_dot_vertex_buffer != nullptr)
        {
            m_dot_vertex_buffer->Release();
            m_dot_vertex_buffer = nullptr;
        }
        if (m_dot_index_buffer != nullptr)
        {
            m_dot_index_buffer->Release();
            m_dot_index_buffer = nullptr;
        }
        if (m_line_vertex_buffer != nullptr)
        {
            m_line_vertex_buffer->Release();
            m_line_vertex_buffer = nullptr;
        }
        if (m_line_index_buffer != nullptr)
        {
            m_line_index_buffer->Release();
            m_line_index_buffer = nullptr;
        }
        if (m_face_vertex_buffer != nullptr)
        {
            m_face_vertex_buffer->Release();
            m_face_vertex_buffer = nullptr;
        }
        if (m_face_index_buffer != nullptr)
        {
            m_face_index_buffer->Release();
            m_face_index_buffer = nullptr;
        }
    }

    void PrimitiveRenderer::UpdatePrimitiveRendererCamera()
    {
        DirectX::XMVECTOR up_vector       = Converter::ToXMVector(Math::Vector3::Y_AXIS);
        DirectX::XMVECTOR pos_vector      = Converter::ToXMVector(m_position);
        DirectX::XMVECTOR look_vector     = Converter::ToXMVector(Math::Vector3::Z_AXIS);
        DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationQuaternion(Converter::ToXMVector(m_rotation));

        // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
        look_vector = DirectX::XMVector3TransformCoord(look_vector, rotation_matrix);
        up_vector   = DirectX::XMVector3TransformCoord(up_vector, rotation_matrix);

        // Translate the rotated camera position to the location of the viewer.
        look_vector = DirectX::XMVectorAdd(pos_vector, look_vector);

        // Finally create the view matrix from the three updated vectors.
        m_view_matrix = DirectX::XMMatrixLookAtLH(pos_vector, look_vector, up_vector);
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

    void PrimitiveRenderer::BuildDotBuffer()
    {
        if (m_dot_vertex_buffer != nullptr)
        {
            m_dot_vertex_buffer->Release();
            m_dot_vertex_buffer = nullptr;
        }
        if (m_dot_index_buffer != nullptr)
        {
            m_dot_index_buffer->Release();
            m_dot_index_buffer = nullptr;
        }
        if (m_dot_vertices.empty() == false)
        {
            // Set up the description of the static vertex buffer.
            D3D11_BUFFER_DESC vertex_buffer_desc;
            vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
            vertex_buffer_desc.ByteWidth           = sizeof(ColorVertex) * static_cast<I32>(m_dot_vertices.size());
            vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            vertex_buffer_desc.CPUAccessFlags      = 0;
            vertex_buffer_desc.MiscFlags           = 0;
            vertex_buffer_desc.StructureByteStride = 0;

            // Give the sub resource structure a pointer to the vertex data.
            D3D11_SUBRESOURCE_DATA vertex_data;
            vertex_data.pSysMem          = m_dot_vertices.data();
            vertex_data.SysMemPitch      = 0;
            vertex_data.SysMemSlicePitch = 0;

            // Now create the vertex buffer.
            HRESULT result = m_dx11_api->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_dot_vertex_buffer);
            if (FAILED(result))
                return;

            // Set up the description of the static index buffer.
            D3D11_BUFFER_DESC index_buffer_desc;
            index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
            index_buffer_desc.ByteWidth           = sizeof(I32) * static_cast<I32>(m_dot_indices.size());;
            index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
            index_buffer_desc.CPUAccessFlags      = 0;
            index_buffer_desc.MiscFlags           = 0;
            index_buffer_desc.StructureByteStride = 0;

            // Give the sub resource structure a pointer to the index data.
            D3D11_SUBRESOURCE_DATA index_data;
            index_data.pSysMem          = m_dot_indices.data();
            index_data.SysMemPitch      = 0;
            index_data.SysMemSlicePitch = 0;

            // Create the index buffer.
            result = m_dx11_api->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_dot_index_buffer);
            if (FAILED(result))
                return;
        }
    }

    void PrimitiveRenderer::UpdateDotBuffer(Real dt) const
    {
        if (m_dot_vertices.empty() == false)
        {
            // Set vertex buffer stride and offset.
            unsigned int stride = sizeof(ColorVertex);
            unsigned int offset = 0;

            // Set the vertex buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetVertexBuffers(0, 1, &m_dot_vertex_buffer, &stride, &offset);

            // Set the index buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetIndexBuffer(m_dot_index_buffer, DXGI_FORMAT_R32_UINT, 0);

            // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
            m_color_shader->Update(dt, m_device_context, static_cast<int>(m_dot_indices.size()), m_world_matrix, m_view_matrix, m_proj_matrix);
        }
    }

    void PrimitiveRenderer::BuildLineBuffer()
    {
        if (m_line_vertex_buffer != nullptr)
        {
            m_line_vertex_buffer->Release();
            m_line_vertex_buffer = nullptr;
        }
        if (m_line_index_buffer != nullptr)
        {
            m_line_index_buffer->Release();
            m_line_index_buffer = nullptr;
        }
        if (m_line_vertices.empty() == false)
        {
            // Set up the description of the static vertex buffer.
            D3D11_BUFFER_DESC vertex_buffer_desc;
            vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
            vertex_buffer_desc.ByteWidth           = sizeof(ColorVertex) * static_cast<I32>(m_line_vertices.size());
            vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            vertex_buffer_desc.CPUAccessFlags      = 0;
            vertex_buffer_desc.MiscFlags           = 0;
            vertex_buffer_desc.StructureByteStride = 0;

            // Give the sub resource structure a pointer to the vertex data.
            D3D11_SUBRESOURCE_DATA vertex_data;
            vertex_data.pSysMem          = m_line_vertices.data();
            vertex_data.SysMemPitch      = 0;
            vertex_data.SysMemSlicePitch = 0;

            // Now create the vertex buffer.
            HRESULT result = m_dx11_api->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_line_vertex_buffer);
            if (FAILED(result))
                return;

            // Set up the description of the static index buffer.
            D3D11_BUFFER_DESC index_buffer_desc;
            index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
            index_buffer_desc.ByteWidth           = sizeof(I32) * static_cast<I32>(m_line_indices.size());;
            index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
            index_buffer_desc.CPUAccessFlags      = 0;
            index_buffer_desc.MiscFlags           = 0;
            index_buffer_desc.StructureByteStride = 0;

            // Give the sub resource structure a pointer to the index data.
            D3D11_SUBRESOURCE_DATA index_data;
            index_data.pSysMem          = m_line_indices.data();
            index_data.SysMemPitch      = 0;
            index_data.SysMemSlicePitch = 0;

            // Create the index buffer.
            result = m_dx11_api->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_line_index_buffer);
            if (FAILED(result))
                return;
        }
    }

    void PrimitiveRenderer::UpdateLineBuffer(Real dt) const
    {
        if (m_line_vertices.empty() == false)
        {
            // Set vertex buffer stride and offset.
            unsigned int stride = sizeof(ColorVertex);
            unsigned int offset = 0;

            // Set the vertex buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetVertexBuffers(0, 1, &m_line_vertex_buffer, &stride, &offset);

            // Set the index buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetIndexBuffer(m_line_index_buffer, DXGI_FORMAT_R32_UINT, 0);

            // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

            m_color_shader->Update(dt, m_device_context, static_cast<int>(m_line_indices.size()), m_world_matrix, m_view_matrix, m_proj_matrix);
        }
    }

    void PrimitiveRenderer::BuildTriangleBuffer()
    {
        if (m_face_vertex_buffer != nullptr)
        {
            m_face_vertex_buffer->Release();
            m_face_vertex_buffer = nullptr;
        }
        if (m_face_index_buffer != nullptr)
        {
            m_face_index_buffer->Release();
            m_face_index_buffer = nullptr;
        }
        if (m_face_vertices.empty() == false)
        {
            // Set up the description of the static vertex buffer.
            D3D11_BUFFER_DESC vertex_buffer_desc;
            vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
            vertex_buffer_desc.ByteWidth           = sizeof(ColorVertex) * static_cast<I32>(m_face_vertices.size());
            vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            vertex_buffer_desc.CPUAccessFlags      = 0;
            vertex_buffer_desc.MiscFlags           = 0;
            vertex_buffer_desc.StructureByteStride = 0;

            // Give the sub resource structure a pointer to the vertex data.
            D3D11_SUBRESOURCE_DATA vertex_data;
            vertex_data.pSysMem          = m_face_vertices.data();
            vertex_data.SysMemPitch      = 0;
            vertex_data.SysMemSlicePitch = 0;

            // Now create the vertex buffer.
            HRESULT result = m_dx11_api->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_face_vertex_buffer);
            if (FAILED(result))
                return;

            // Set up the description of the static index buffer.
            D3D11_BUFFER_DESC index_buffer_desc;
            index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
            index_buffer_desc.ByteWidth           = sizeof(I32) * static_cast<I32>(m_face_indices.size());;
            index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
            index_buffer_desc.CPUAccessFlags      = 0;
            index_buffer_desc.MiscFlags           = 0;
            index_buffer_desc.StructureByteStride = 0;

            // Give the sub resource structure a pointer to the index data.
            D3D11_SUBRESOURCE_DATA index_data;
            index_data.pSysMem          = m_face_indices.data();
            index_data.SysMemPitch      = 0;
            index_data.SysMemSlicePitch = 0;

            // Create the index buffer.
            result = m_dx11_api->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_face_index_buffer);
            if (FAILED(result))
                return;
        }
    }

    void PrimitiveRenderer::UpdateTriangleBuffer(Real dt) const
    {
        if (m_face_vertices.empty() == false)
        {
            // Set vertex buffer stride and offset.
            unsigned int stride = sizeof(ColorVertex);
            unsigned int offset = 0;

            // Set the vertex buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetVertexBuffers(0, 1, &m_face_vertex_buffer, &stride, &offset);

            // Set the index buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetIndexBuffer(m_face_index_buffer, DXGI_FORMAT_R32_UINT, 0);

            // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            m_color_shader->Update(dt, m_device_context, static_cast<int>(m_face_indices.size()), m_world_matrix, m_view_matrix, m_proj_matrix);
        }
    }

    void PrimitiveRenderer::UpdateProjectionMatrix()
    {
        m_proj_matrix = m_dx11_api->GetProjectionMatrix();
    }

    void PrimitiveRenderer::SetRendererCameraPosition(const Vector3& pos)
    {
        m_position = pos;
        this->UpdatePrimitiveRendererCamera();
    }

    void PrimitiveRenderer::SetRendererCameraRotation(const Quaternion& rot)
    {
        m_rotation = rot;
        this->UpdatePrimitiveRendererCamera();
    }

    void PrimitiveRenderer::PushVertex(const Vector3& pos, RenderingMode mode, const Color& color)
    {
        switch (mode)
        {
        case Engine5::RenderingMode::Dot:
            m_dot_vertices.emplace_back(pos, color);
            break;
        case Engine5::RenderingMode::Line:
            m_line_vertices.emplace_back(pos, color);
            break;
        case Engine5::RenderingMode::Face:
            m_face_vertices.emplace_back(pos, color);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::PushIndex(I32 index, RenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::RenderingMode::Dot:
            m_dot_indices.push_back(index);
            break;
        case Engine5::RenderingMode::Line:
            m_line_indices.push_back(index);
            break;
        case Engine5::RenderingMode::Face:
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

    void PrimitiveRenderer::ReserveVertices(size_t adding_count, RenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::RenderingMode::Dot:
            m_dot_vertices.reserve(m_dot_vertices.size() + adding_count);
            break;
        case Engine5::RenderingMode::Line:
            m_line_vertices.reserve(m_line_vertices.size() + adding_count);
            break;
        case Engine5::RenderingMode::Face:
            m_face_vertices.reserve(m_face_vertices.size() + adding_count);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::ReserveIndices(size_t adding_count, RenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::RenderingMode::Dot:
            m_dot_indices.reserve(m_dot_indices.size() + adding_count);
            break;
        case Engine5::RenderingMode::Line:
            m_line_indices.reserve(m_line_indices.size() + adding_count);
            break;
        case Engine5::RenderingMode::Face:
            m_face_indices.reserve(m_face_indices.size() + adding_count);
            break;
        default:
            break;
        }
    }

    size_t PrimitiveRenderer::VerticesSize(RenderingMode mode) const
    {
        switch (mode)
        {
        case Engine5::RenderingMode::Dot:
            return m_dot_vertices.size();
        case Engine5::RenderingMode::Line:
            return m_line_vertices.size();
        case Engine5::RenderingMode::Face:
            return m_face_vertices.size();
        default:
            break;
        }
        return 0;
    }

    size_t PrimitiveRenderer::IndicesSize(RenderingMode mode) const
    {
        switch (mode)
        {
        case Engine5::RenderingMode::Dot:
            return m_dot_indices.size();
        case Engine5::RenderingMode::Line:
            return m_line_indices.size();
        case Engine5::RenderingMode::Face:
            return m_face_indices.size();
        default:
            break;
        }
        return 0;
    }
}

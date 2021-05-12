#include "ShaderProgramDX11.hpp"

#include "../../Common/Buffer2/VertexLayoutCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../Common/Shader/ShaderProgramCommon.hpp"

namespace Engine5
{
    ShaderProgramDX11::ShaderProgramDX11()
    {
    }

    ShaderProgramDX11::~ShaderProgramDX11()
    {
    }

    ShaderProgramCommon::ShaderProgramCommon()
    {
    }

    ShaderProgramCommon::~ShaderProgramCommon()
    {
    }

    bool ShaderProgramCommon::Init(RendererCommon* renderer)
    {
        ID3D10Blob* error_message        = nullptr;
        ID3D10Blob* vertex_shader_buffer = nullptr;
        ID3D10Blob* pixel_shader_buffer  = nullptr;
        //Initialize vertex shader

        //Initialize pixel shader

        //Initialize input layout
        if (m_vertex_layout == nullptr)
            return false;

        auto   attributes = m_vertex_layout->GetAttributes();
        size_t size       = attributes.size();

        if (size == 0)
            return false;

        std::vector<D3D11_INPUT_ELEMENT_DESC> vertex_layout;
        vertex_layout.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            vertex_layout[i].SemanticName         = attributes[i].sem_name.c_str();
            vertex_layout[i].SemanticIndex        = attributes[i].sem_idx;
            vertex_layout[i].InputSlot            = attributes[i].input_slot;
            vertex_layout[i].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
            vertex_layout[i].InputSlotClass       = (D3D11_INPUT_CLASSIFICATION)attributes[i].slot_type;
            vertex_layout[i].InstanceDataStepRate = attributes[i].step_rate;

            size_t count = attributes[i].format_count;
            auto   type  = attributes[i].format_type;

            int format;
            switch (count)
            {
            case 2:
                format = (int)DXGI_FORMAT_R32G32_TYPELESS;
                break;
            case 3:
                format = (int)DXGI_FORMAT_R32G32B32_TYPELESS;
                break;
            case 4:
                format = (int)DXGI_FORMAT_R32G32B32A32_TYPELESS;
                break;
            default:
                format = (int)DXGI_FORMAT_UNKNOWN;
                break;
            }

            switch (type)
            {
            case eAttributeType::T32:
                break;
            case eAttributeType::R32:
                format += 1;
                break;
            case eAttributeType::I32:
                format += 3;
                break;
            case eAttributeType::U32:
                format += 2;
                break;
            default:
                break;
            }

            vertex_layout[i].Format = (DXGI_FORMAT)format;
        }

        HRESULT result = renderer->GetDevice()->CreateInputLayout(
                                                                  vertex_layout.data(), (UINT)vertex_layout.size(),
                                                                  vertex_shader_buffer->GetBufferPointer(),
                                                                  vertex_shader_buffer->GetBufferSize(), &m_layout);
        if (FAILED(result))
        {
            return false;
        }

        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void ShaderProgramCommon::Shutdown()
    {
        m_vertex_layout = nullptr;
    }

    void ShaderProgramCommon::Bind()
    {
    }

    void ShaderProgramCommon::SetVertexLayout(VertexLayoutCommon* layout)
    {
        m_vertex_layout = layout;
    }
}

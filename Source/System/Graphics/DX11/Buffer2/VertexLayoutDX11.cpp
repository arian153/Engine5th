#include "VertexLayoutDX11.hpp"

#include "../../../Core/Utility/CoreDef.hpp"
#include "../../Common/Buffer2/VertexLayoutCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace Engine5
{
    VertexLayoutDX11::VertexLayoutDX11()
    {
    }

    VertexLayoutDX11::~VertexLayoutDX11()
    {
    }

    VertexAttributes::VertexAttributes(eAttributeType _type, U32 _count, bool _b_normalized)
        : format_type(_type), format_count(_count), b_normalized(_b_normalized)
    {
    }

    VertexAttributes::VertexAttributes(eAttributeType _type, U32 _count, const std::string& semantic_name, U32 semantic_idx)
        : format_type(_type), format_count(_count), sem_name(semantic_name), sem_idx(semantic_idx)
    {
    }

    VertexAttributes::VertexAttributes(eAttributeType _type, U32 _count, const std::string& semantic_name, U32 semantic_idx, eInputSlotType _slot_type, U32 _input_slot, U32 _step_rate)
        : format_type(_type), format_count(_count), sem_name(semantic_name), sem_idx(semantic_idx), slot_type(_slot_type), input_slot(_input_slot), step_rate(_step_rate)
    {
    }

    U32 VertexAttributes::SizeOf(eAttributeType type)
    {
        switch (type)
        {
        case eAttributeType::T32:
            return sizeof(int);
        case eAttributeType::R32:
            return sizeof(R32);
        case eAttributeType::I32:
            return sizeof(I32);
        case eAttributeType::U32:
            return sizeof(U32);
        default:
            return 0;
        }
    }

    VertexLayoutCommon::VertexLayoutCommon()
    {
    }

    VertexLayoutCommon::~VertexLayoutCommon()
    {
    }

    bool VertexLayoutCommon::Init(RendererCommon* renderer, void* shader_data)
    {
        size_t size = m_attributes.size();
        if (size == 0)
            return false;

        std::vector<D3D11_INPUT_ELEMENT_DESC> vertex_layout;
        vertex_layout.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            vertex_layout[i].SemanticName         = m_attributes[i].sem_name.c_str();
            vertex_layout[i].SemanticIndex        = m_attributes[i].sem_idx;
            vertex_layout[i].InputSlot            = m_attributes[i].input_slot;
            vertex_layout[i].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
            vertex_layout[i].InputSlotClass       = (D3D11_INPUT_CLASSIFICATION)m_attributes[i].slot_type;
            vertex_layout[i].InstanceDataStepRate = m_attributes[i].step_rate;

            size_t count = m_attributes[i].format_count;
            auto   type  = m_attributes[i].format_type;

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

        ID3D10Blob* vertex_shader_buffer = (ID3D10Blob*)shader_data;
        HRESULT     result               = renderer->GetDevice()->CreateInputLayout(
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

    void VertexLayoutCommon::Shutdown()
    {
        if (m_layout != nullptr)
        {
            m_layout->Release();
            m_layout = nullptr;
        }
    }

    void VertexLayoutCommon::Bind() const
    {
        if (m_device_context != nullptr)
        {
            m_device_context->IASetInputLayout(m_layout);
        }
    }

    void VertexLayoutCommon::Push(eAttributeType type, U32 count, bool is_normalized)
    {
        m_attributes.emplace_back(type, count, is_normalized);
    }

    void VertexLayoutCommon::Push(eAttributeType type, U32 count, const std::string& semantic_name, U32 semantic_idx)
    {
        m_attributes.emplace_back(type, count, semantic_name, semantic_idx);
    }

    void VertexLayoutCommon::Push(eAttributeType type, U32 count, const std::string& semantic_name, U32 semantic_idx, eInputSlotType slot_type, U32 input_slot, U32 step_rate)
    {
        m_attributes.emplace_back(type, count, semantic_name, semantic_idx, slot_type, input_slot, step_rate);
    }

    void VertexLayoutCommon::Push(VertexAttributes attributes)
    {
        m_attributes.push_back(attributes);
    }
}

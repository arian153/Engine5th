#include "VertexLayoutDX11.hpp"

#include "../../Common/Buffer2/VertexLayoutCommon.hpp"

namespace Engine5
{
    VertexLayoutDX11::VertexLayoutDX11()
    {
    }

    VertexLayoutDX11::~VertexLayoutDX11()
    {
    }

    VertexAttributes::VertexAttributes(const std::string& _name, eAttributeType _type, size_t _count, bool _b_normalized)
        : name(_name), type(_type), count(_count), b_normalized(_b_normalized)
    {
    }

    VertexLayoutCommon::VertexLayoutCommon()
    {
    }

    VertexLayoutCommon::~VertexLayoutCommon()
    {
    }

    bool VertexLayoutCommon::Init()
    {
        size_t size = m_attributes.size();
        if (size == 0)
            return false;

        std::vector<D3D11_INPUT_ELEMENT_DESC> vertex_layout;
        vertex_layout.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            vertex_layout[i].SemanticName  = m_attributes[i].name.c_str();
            vertex_layout[i].SemanticIndex = 0;

            vertex_layout[i].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
            vertex_layout[i].InputSlot            = 0;
            vertex_layout[i].AlignedByteOffset    = 0;
            vertex_layout[i].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
            vertex_layout[i].InstanceDataStepRate = 0;

            size_t count = m_attributes[i].count;
            auto   type  = m_attributes[i].type;
            switch (type)
            {
            case Engine5::eAttributeType::R32:
                vertex_layout[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                break;
            case Engine5::eAttributeType::I8:
                break;
            case Engine5::eAttributeType::I16:
                break;
            case Engine5::eAttributeType::I32:
                break;
            case Engine5::eAttributeType::U8:
                break;
            case Engine5::eAttributeType::U16:
                break;
            case Engine5::eAttributeType::U32:
                break;
            default:
                break;
            }
        }

        return true;
    }

    void VertexLayoutCommon::Shutdown()
    {
    }

    void VertexLayoutCommon::Bind()
    {
    }

    void VertexLayoutCommon::Push(const std::string& name, eAttributeType type, size_t count, bool is_normalized)
    {
        m_attributes.emplace_back(name, type, count, is_normalized);
    }

    void VertexLayoutCommon::Push(VertexAttributes attributes)
    {
        m_attributes.push_back(attributes);
    }
}

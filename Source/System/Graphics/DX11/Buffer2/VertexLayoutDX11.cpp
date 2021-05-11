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
            vertex_layout[i].SemanticName         = m_attributes[i].name.c_str();
            vertex_layout[i].SemanticIndex        = 0;
            vertex_layout[i].InputSlot            = 0;
            vertex_layout[i].AlignedByteOffset    = 0;
            vertex_layout[i].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
            vertex_layout[i].InstanceDataStepRate = 0;

            size_t count = m_attributes[i].count;
            auto   type  = m_attributes[i].type;

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
            case eAttributeType::F32:
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

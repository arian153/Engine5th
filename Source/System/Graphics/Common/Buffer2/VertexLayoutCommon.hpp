#pragma once

#include <string>
#include <vector>

#include "../../GraphicsAPI.hpp"
#include IncludeVertexLayoutAPI

namespace Engine5
{
    enum class eAttributeType : int
    {
        R32,
        I8,
        I16,
        I32,
        U8,
        U16,
        U32
    };

    class VertexAttributes
    {
    public:
        VertexAttributes()  = default;
        ~VertexAttributes() = default;
        VertexAttributes(const std::string& _name, eAttributeType _type, size_t _count, bool _b_normalized);

    public:
        std::string    name         = "";
        eAttributeType type         = eAttributeType::R32;
        size_t         count        = 0;
        bool           b_normalized = false;
    };

    class VertexLayoutCommon : public VertexLayoutAPI
    {
    public:
        VertexLayoutCommon();
        ~VertexLayoutCommon();

        bool Init();
        void Shutdown();
        void Bind();

        void Push(const std::string& name, eAttributeType type, size_t count, bool is_normalized);
        void Push(VertexAttributes attributes);

    private:
        std::vector<VertexAttributes> m_attributes;
    };
}

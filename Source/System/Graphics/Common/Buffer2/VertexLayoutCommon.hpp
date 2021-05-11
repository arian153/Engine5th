#pragma once

#include <string>
#include <vector>

#include "../../GraphicsAPI.hpp"
#include IncludeVertexLayoutAPI

namespace Engine5
{
    class RendererCommon;

    enum class eAttributeType : int
    {
        T32,
        R32,
        I32,
        U32
    };

    enum class eInputSlotType : int
    {
        VERTEX_DATA,
        INSTANCE_DATA
    };

    class VertexAttributes
    {
    public:
        VertexAttributes()  = default;
        ~VertexAttributes() = default;
        VertexAttributes(eAttributeType _type, U32 _count, bool _b_normalized);
        VertexAttributes(eAttributeType _type, U32 _count, const std::string& semantic_name, U32 semantic_idx);
        VertexAttributes(eAttributeType _type, U32 _count, const std::string& semantic_name, U32 semantic_idx, eInputSlotType _slot_type, U32 _input_slot, U32 _step_rate);

        static U32 SizeOf(eAttributeType type);

    public:
        eAttributeType format_type  = eAttributeType::R32;
        U32            format_count = 0;

    public:
        std::string    sem_name     = "";
        U32            sem_idx      = 0;
        eInputSlotType slot_type    = eInputSlotType::VERTEX_DATA;
        U32            input_slot   = 0;
        U32            step_rate    = 0;
        bool           b_normalized = false;
    };

    class VertexLayoutCommon : public VertexLayoutAPI
    {
    public:
        VertexLayoutCommon();
        ~VertexLayoutCommon();

        bool Init(RendererCommon* renderer, void* shader_data);
        void Shutdown();
        void Bind() const;

        void Push(eAttributeType type, U32 count, bool is_normalized);
        void Push(eAttributeType type, U32 count, const std::string& semantic_name, U32 semantic_idx);
        void Push(eAttributeType type, U32 count, const std::string& semantic_name, U32 semantic_idx, eInputSlotType slot_type, U32 input_slot, U32 step_rate);
        void Push(VertexAttributes attributes);

    private:
        std::vector<VertexAttributes> m_attributes;
    };
}

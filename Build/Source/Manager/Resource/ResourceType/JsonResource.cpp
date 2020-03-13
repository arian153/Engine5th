#include "JsonResource.hpp"

namespace Engine5
{
    JsonResource::JsonResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Json;
    }

    JsonResource::~JsonResource()
    {
    }

    bool JsonResource::IsLevel() const
    {
        return m_json_type == eJsonType::Level;
    }

    bool JsonResource::IsSpace() const
    {
        return m_json_type == eJsonType::Space;
    }

    bool JsonResource::IsArchetype() const
    {
        return m_json_type == eJsonType::Archetype;
    }

    bool JsonResource::IsObject() const
    {
        return m_json_type == eJsonType::Object;
    }

    bool JsonResource::IsMeshData() const
    {
        return m_json_type == eJsonType::MeshData;
    }

    bool JsonResource::IsParticleData() const
    {
        return m_json_type == eJsonType::ParticleData;
    }

    eJsonType JsonResource::GetType() const
    {
        return m_json_type;
    }

    void JsonResource::LoadType()
    {

    }
}

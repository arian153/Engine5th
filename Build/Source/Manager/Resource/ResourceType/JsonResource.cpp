#include "JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include <fstream>

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

    void JsonResource::Initialize()
    {
        Json::CharReaderBuilder builder;
        m_reader = builder.newCharReader();
        LoadType();
    }

    void JsonResource::Shutdown()
    {
        if (m_reader != nullptr)
        {
            delete m_reader;
            m_reader = nullptr;
        }
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

    bool JsonResource::LoadType()
    {
        Json::Value   root;
        std::ifstream file(m_file_path, std::ifstream::binary);
        std::string   doc;
        std::getline(file, doc, static_cast<char>(EOF));
        bool b_parsing_successful = m_reader->parse(doc.data(), doc.data() + doc.size(), &root, nullptr);
        if (b_parsing_successful)
        {
            if (HasMember(root, "Type") && root["Type"].isString())
            {
                std::string json_type = root["Type"].asString();
                if (json_type == "Level")
                {
                    m_json_type = eJsonType::Level;
                }
                else if (json_type == "Space")
                {
                    m_json_type = eJsonType::Space;
                }
                else if (json_type == "Object")
                {
                    m_json_type = eJsonType::Object;
                }
                else if (json_type == "Archetype")
                {
                    m_json_type = eJsonType::Archetype;
                }
                else if (json_type == "MeshData")
                {
                    m_json_type = eJsonType::MeshData;
                }
                else if (json_type == "ParticleData")
                {
                    m_json_type = eJsonType::ParticleData;
                }
                else
                {
                    m_json_type = eJsonType::None;
                }
                return true;
            }
        }
        return false;
    }

    bool JsonResource::HasMember(const Json::Value& data, const std::string& find) const
    {
        return !(data[find].isNull());
    }
}

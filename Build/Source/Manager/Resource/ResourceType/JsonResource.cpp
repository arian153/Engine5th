#include "JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include <fstream>
#include "../../Level/Level.hpp"
#include "../ResourceManager.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"

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
        m_reader    = builder.newCharReader();
        m_root_data = new Json::Value();
        LoadJsonType();
    }

    void JsonResource::Shutdown()
    {
        if (m_reader != nullptr)
        {
            delete m_reader;
            m_reader = nullptr;
        }
        if (m_root_data != nullptr)
        {
            delete m_root_data;
            m_root_data = nullptr;
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

    bool JsonResource::LoadJsonType()
    {
        std::ifstream file(m_file_path, std::ifstream::binary);
        std::string   doc;
        std::getline(file, doc, static_cast<char>(EOF));
        bool b_parsing_successful = m_reader->parse(doc.data(), doc.data() + doc.size(), m_root_data, nullptr);
        if (b_parsing_successful)
        {
            if (HasMember(*m_root_data, "Type") && (*m_root_data)["Type"].isString())
            {
                std::string json_type = (*m_root_data)["Type"].asString();
                if (json_type == "Level")
                {
                    m_json_type = eJsonType::Level;
                }
                else if (json_type == "Space")
                {
                    m_json_type = eJsonType::Space;
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

    bool JsonResource::LoadData(Level* level) const
    {
        if (HasMember(*m_root_data, "Spaces") && (*m_root_data)["Spaces"].isArray())
        {
            for (auto it = (*m_root_data)["Spaces"].begin(); it != (*m_root_data)["Spaces"].end(); ++it)
            {
                if (it->isString())
                {
                    std::wstring path           = m_resource_manager->GetRootPath() + StringToWString(it->asString());
                    auto         space_resource = m_resource_manager->GetJsonResource(path);
                    level->AddSpaceResource(space_resource);
                }
            }
        }
        return true;
    }

    bool JsonResource::LoadData(Space* space) const
    {
        if (HasMember(*m_root_data, "Objects") && (*m_root_data)["Objects"].isArray())
        {
            for (auto it = (*m_root_data)["Objects"].begin(); it != (*m_root_data)["Objects"].end(); ++it)
            {
                //Load Object
            }
        }
        if (HasMember(*m_root_data, "Settings"))
        {
            Json::Value setting = (*m_root_data)["Settings"];
            if (HasMember(setting, "ProjectionMatrix"))
            {
                //Load Setting
            }
            for (auto it = (*m_root_data)["Objects"].begin(); it != (*m_root_data)["Objects"].end(); ++it)
            {
                //Load Object
            }
        }
        return true;
    }

    bool JsonResource::LoadSpaceFlag(Space* space) const
    {
        if (HasMember(*m_root_data, "Flag") && (*m_root_data)["Flag"].isArray())
        {
            for (auto it = (*m_root_data)["Flag"].begin(); it != (*m_root_data)["Flag"].end(); ++it)
            {
                if (it->isString())
                {
                    std::string flag = it->asString();
                    if (flag == "ComponentManager")
                    {
                        space->m_creation_flag |= eSubsystemFlag::ComponentManager;
                    }
                    else if (flag == "ObjectManager")
                    {
                        space->m_creation_flag |= eSubsystemFlag::ObjectManager;
                    }
                    else if (flag == "Scene")
                    {
                        space->m_creation_flag |= eSubsystemFlag::Scene;
                    }
                    else if (flag == "World")
                    {
                        space->m_creation_flag |= eSubsystemFlag::World;
                    }
                }
            }
        }
        return true;
    }
}

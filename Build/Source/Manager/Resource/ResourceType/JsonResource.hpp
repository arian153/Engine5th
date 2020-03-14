#pragma once
#include "../Resource.hpp"

namespace Json {
    class CharReader;
    class Value;
}

namespace Engine5
{
    enum class eJsonType : size_t
    {
        None
      , Level
      , Space
      , Archetype
      , Object
      , MeshData
      , ParticleData
    };

    class JsonResource final : public Resource
    {
    public:
        explicit JsonResource(const std::wstring& path);
        ~JsonResource();

        void Initialize() override;
        void Shutdown() override;

        bool      IsLevel() const;
        bool      IsSpace() const;
        bool      IsArchetype() const;
        bool      IsObject() const;
        bool      IsMeshData() const;
        bool      IsParticleData() const;
        eJsonType GetType() const;

        bool LoadType();

        bool HasMember(const Json::Value& data, const std::string& find) const;

    private:
        eJsonType m_json_type = eJsonType::None;
        Json::CharReader* m_reader = nullptr;
    };
}

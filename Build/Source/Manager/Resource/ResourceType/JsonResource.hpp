#pragma once
#include "../Resource.hpp"

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

        bool      IsLevel() const;
        bool      IsSpace() const;
        bool      IsArchetype() const;
        bool      IsObject() const;
        bool      IsMeshData() const;
        bool      IsParticleData() const;
        eJsonType GetType() const;

        void LoadType();

    private:
        eJsonType m_json_type = eJsonType::None;
    };
}

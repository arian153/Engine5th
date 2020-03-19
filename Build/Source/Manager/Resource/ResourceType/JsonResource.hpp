#pragma once
#include "../Resource.hpp"

namespace Json
{
    class CharReader;
    class Value;
}

namespace Engine5
{
    class Object;
    struct ApplicationSetting;
    class Space;
    class Level;

    enum class eJsonType : size_t
    {
        None
      , Setting
      , Level
      , Space
      , Archetype
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

        bool      IsSetting() const;
        bool      IsLevel() const;
        bool      IsSpace() const;
        bool      IsArchetype() const;
        bool      IsMeshData() const;
        bool      IsParticleData() const;
        eJsonType GetType() const;
        bool      LoadJsonType();

    public:
        static bool HasMember(const Json::Value& data, const std::string& find);

    private:
        bool LoadSetting(ApplicationSetting& app_setting) const;
        bool LoadData(Level* level) const;
        bool LoadData(Space* space) const;
        bool LoadSpaceFlag(Space* space) const;

        //Create Empty Json Data
        void CreateLevelData(const std::wstring& space_path) const;
        void CreateSpaceData() const;

    private:
        friend class Space;
        friend class Level;
        friend class LevelManager;
        friend class Application;

    private:
        eJsonType         m_json_type   = eJsonType::None;
        Json::CharReader* m_reader      = nullptr;
        Json::Value*      m_root_data   = nullptr;
        bool              m_b_load_type = true;
    };
}

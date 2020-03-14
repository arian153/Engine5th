#pragma once
#include <string>
#include "ResourceType.hpp"

namespace Engine5
{
    class Resource
    {
    public:
        explicit Resource(const std::wstring& path);
        virtual  ~Resource();

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        bool IsLoaded() const;
        bool IsUnloaded() const;

        std::wstring FileName() const;
        std::wstring FilePath() const;
        std::wstring FileType() const;

    protected:
        std::wstring  m_file_path;
        std::wstring  m_file_name;
        std::wstring  m_file_type;
        eResourceType m_type       = eResourceType::Undefined;
        bool          m_b_loaded   = false;
        bool          m_b_unloaded = false;

    private:
        friend class ResourceManager;
    };
}

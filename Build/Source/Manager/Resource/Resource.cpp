#include "Resource.hpp"

namespace Engine5
{
    Resource::Resource(const std::wstring& path)
        : m_file_path(path)
    {
    }

    Resource::~Resource()
    {
    }

    bool Resource::IsLoaded() const
    {
        return m_b_loaded;
    }

    bool Resource::IsUnloaded() const
    {
        return m_b_unloaded;
    }

    std::wstring Resource::FileName() const
    {
        return m_file_name;
    }

    std::wstring Resource::FilePath() const
    {
        return m_file_path;
    }

    std::wstring Resource::FileType() const
    {
        return m_file_type;
    }
}

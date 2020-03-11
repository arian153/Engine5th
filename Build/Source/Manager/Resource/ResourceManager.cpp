#include "ResourceManager.hpp"

namespace Engine5
{
    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    void ResourceManager::Initialize()
    {
    }

    void ResourceManager::Shutdown()
    {
    }

    Resource* ResourceManager::GetResource(const std::wstring& path)
    {
    }

    void ResourceManager::RemoveResource(const std::wstring& path)
    {
    }

    void ResourceManager::RemoveResource(Resource* resource)
    {
    }

    void ResourceManager::ClearResource()
    {
    }

    void ResourceManager::BuildResource()
    {
    }

    void ResourceManager::QueryFilePathRecursive(const std::wstring& path, std::list<std::wstring>& file_list, std::list<std::wstring>& directory_list)
    {
    }

    Resource* ResourceManager::AddResource(const std::wstring& path)
    {
    }
}

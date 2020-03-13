// ReSharper disable CppExpressionWithoutSideEffects
#include "ResourceManager.hpp"
#include "Resource.hpp"
#include "../../System/Core/Utility/FileUtility.hpp"
#include "ResourceType/AudioResource.hpp"
#include "ResourceType/TextureResource.hpp"
#include "ResourceType/MeshResource.hpp"
#include "ResourceType/ShaderResource.hpp"
#include "ResourceType/JsonResource.hpp"

namespace Engine5
{
    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    void ResourceManager::Initialize(FileUtility* file_utility)
    {
        m_file_utility = file_utility;
        BuildResource();
    }

    void ResourceManager::Shutdown()
    {
        m_file_list.clear();
        m_directory_list.clear();
        ClearResource();
    }

    Resource* ResourceManager::GetResource(const std::wstring& path)
    {
        auto found = m_other_resource_map.find(path);
        if (found != m_other_resource_map.end())
        {
            return found->second;
        }
        return AddResource(path);
    }

    Resource* ResourceManager::GetResourceFileName(const std::wstring& file_name)
    {
        for (auto& other : m_other_resource_map)
        {
            auto resource = other.second;
            if (resource->FileName() + resource->FileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetResources(const std::wstring& file_name, std::vector<Resource*>& resources)
    {
        for (auto& other : m_other_resource_map)
        {
            auto resource = other.second;
            if (resource->FileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->FileName() + resource->FileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    void ResourceManager::RemoveResource(const std::wstring& path)
    {
        auto found_in_resource = m_other_resource_map.find(path);
        if (found_in_resource != m_other_resource_map.end())
        {
            if (found_in_resource->second->IsLoaded() == true &&
                found_in_resource->second->IsUnloaded() == true)
            {
                if (found_in_resource->second != nullptr)
                {
                    delete found_in_resource->second;
                    found_in_resource->second = nullptr;
                }
                m_other_resource_map.erase(found_in_resource);
            }
        }

        auto found_in_json = m_json_resource_map.find(path);
        if (found_in_json != m_json_resource_map.end())
        {
            if (found_in_json->second->IsLoaded() == true &&
                found_in_json->second->IsUnloaded() == true)
            {
                if (found_in_json->second != nullptr)
                {
                    delete found_in_json->second;
                    found_in_json->second = nullptr;
                }
                m_json_resource_map.erase(found_in_json);
            }
        }

        auto found_in_audio = m_audio_resource_map.find(path);
        if (found_in_audio != m_audio_resource_map.end())
        {
            if (found_in_audio->second->IsLoaded() == true &&
                found_in_audio->second->IsUnloaded() == true)
            {
                if (found_in_audio->second != nullptr)
                {
                    delete found_in_audio->second;
                    found_in_audio->second = nullptr;
                }
                m_audio_resource_map.erase(found_in_audio);
            }
        }

        auto found_in_texture = m_texture_resource_map.find(path);
        if (found_in_texture != m_texture_resource_map.end())
        {
            if (found_in_texture->second->IsLoaded() == true &&
                found_in_texture->second->IsUnloaded() == true)
            {
                if (found_in_texture->second != nullptr)
                {
                    delete found_in_texture->second;
                    found_in_texture->second = nullptr;
                }
                m_texture_resource_map.erase(found_in_texture);
            }
        }
        auto found_in_mesh = m_mesh_resource_map.find(path);
        if (found_in_mesh != m_mesh_resource_map.end())
        {
            if (found_in_mesh->second->IsLoaded() == true &&
                found_in_mesh->second->IsUnloaded() == true)
            {
                if (found_in_mesh->second != nullptr)
                {
                    delete found_in_mesh->second;
                    found_in_mesh->second = nullptr;
                }
                m_mesh_resource_map.erase(found_in_mesh);
            }
        }
        auto found_in_shader = m_shader_resource_map.find(path);
        if (found_in_shader != m_shader_resource_map.end())
        {
            if (found_in_shader->second->IsLoaded() == true &&
                found_in_shader->second->IsUnloaded() == true)
            {
                if (found_in_shader->second != nullptr)
                {
                    delete found_in_shader->second;
                    found_in_shader->second = nullptr;
                }
                m_shader_resource_map.erase(found_in_shader);
            }
        }
    }

    void ResourceManager::RemoveResource(Resource* resource)
    {
        RemoveResource(resource->FilePath());
    }

    void ResourceManager::ClearResource()
    {
        m_file_utility->RemoveFile(L"Resource Manager Report.txt");
        for (auto& resource : m_shader_resource_map)
        {
            if (resource.second != nullptr)
            {
                if (resource.second->IsLoaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (resource.second->IsUnloaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete resource.second;
                resource.second = nullptr;
            }
        }
        m_shader_resource_map.clear();
        for (auto& resource : m_mesh_resource_map)
        {
            if (resource.second != nullptr)
            {
                if (resource.second->IsLoaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (resource.second->IsUnloaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete resource.second;
                resource.second = nullptr;
            }
        }
        m_mesh_resource_map.clear();
        for (auto& resource : m_texture_resource_map)
        {
            if (resource.second != nullptr)
            {
                if (resource.second->IsLoaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (resource.second->IsUnloaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete resource.second;
                resource.second = nullptr;
            }
        }
        m_texture_resource_map.clear();
        for (auto& resource : m_audio_resource_map)
        {
            if (resource.second != nullptr)
            {
                if (resource.second->IsLoaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (resource.second->IsUnloaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete resource.second;
                resource.second = nullptr;
            }
        }
        m_audio_resource_map.clear();
        for (auto& resource : m_json_resource_map)
        {
            if (resource.second != nullptr)
            {
                if (resource.second->IsLoaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (resource.second->IsUnloaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete resource.second;
                resource.second = nullptr;
            }
        }
        m_json_resource_map.clear();
        //remove remains
        for (auto& resource : m_other_resource_map)
        {
            if (resource.second != nullptr)
            {
                if (resource.second->IsLoaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (resource.second->IsUnloaded() == false)
                {
                    std::wstring result = resource.second->FileName() + resource.second->FileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete resource.second;
                resource.second = nullptr;
            }
        }
        m_other_resource_map.clear();
    }

    void ResourceManager::BuildResource()
    {
        m_file_list.clear();
        m_directory_list.clear();
        QueryFilePathRecursive(L"../../Resource", m_file_list, m_directory_list);
        for (auto& file : m_file_list)
        {
            AddResource(file);
        }
    }

    ShaderResource* ResourceManager::GetShaderResource(const std::wstring& path)
    {
        auto found = m_shader_resource_map.find(path);
        if (found != m_shader_resource_map.end())
        {
            return found->second;
        }
        return static_cast<ShaderResource*>(AddResource(path));
    }

    ShaderResource* ResourceManager::GetShaderResourceFileName(const std::wstring& file_name)
    {
        for (auto& shader : m_shader_resource_map)
        {
            auto resource = shader.second;
            if (resource->FileName() + resource->FileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetShaderResources(const std::wstring& file_name, std::vector<ShaderResource*>& resources)
    {
        for (auto& shader : m_shader_resource_map)
        {
            auto resource = shader.second;
            if (resource->FileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->FileName() + resource->FileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    TextureResource* ResourceManager::GetTextureResource(const std::wstring& path)
    {
        auto found = m_texture_resource_map.find(path);
        if (found != m_texture_resource_map.end())
        {
            return found->second;
        }
        return static_cast<TextureResource*>(AddResource(path));
    }

    TextureResource* ResourceManager::GetTextureResourceFileName(const std::wstring& file_name)
    {
        for (auto& texture : m_texture_resource_map)
        {
            auto resource = texture.second;
            if (resource->FileName() + resource->FileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetTextureResources(const std::wstring& file_name, std::vector<TextureResource*>& resources)
    {
        for (auto& texture : m_texture_resource_map)
        {
            auto resource = texture.second;
            if (resource->FileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->FileName() + resource->FileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    MeshResource* ResourceManager::GetMeshResource(const std::wstring& path)
    {
        auto found = m_mesh_resource_map.find(path);
        if (found != m_mesh_resource_map.end())
        {
            return found->second;
        }
        return static_cast<MeshResource*>(AddResource(path));
    }

    MeshResource* ResourceManager::GetMeshResourceFileName(const std::wstring& file_name)
    {
        for (auto& mesh : m_mesh_resource_map)
        {
            auto resource = mesh.second;
            if (resource->FileName() + resource->FileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetMeshResources(const std::wstring& file_name, std::vector<MeshResource*>& resources)
    {
        for (auto& mesh : m_mesh_resource_map)
        {
            auto resource = mesh.second;
            if (resource->FileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->FileName() + resource->FileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    AudioResource* ResourceManager::GetAudioResource(const std::wstring& path)
    {
        auto found = m_audio_resource_map.find(path);
        if (found != m_audio_resource_map.end())
        {
            return found->second;
        }
        return static_cast<AudioResource*>(AddResource(path));
    }

    AudioResource* ResourceManager::GetAudioResourceFileName(const std::wstring& file_name)
    {
        for (auto& audio : m_audio_resource_map)
        {
            auto resource = audio.second;
            if (resource->FileName() + resource->FileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetAudioResources(const std::wstring& file_name, std::vector<AudioResource*>& resources)
    {
        for (auto& audio : m_audio_resource_map)
        {
            auto resource = audio.second;
            if (resource->FileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->FileName() + resource->FileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    JsonResource* ResourceManager::GetJsonResource(const std::wstring& path)
    {
        auto found = m_json_resource_map.find(path);
        if (found != m_json_resource_map.end())
        {
            return found->second;
        }
        return static_cast<JsonResource*>(AddResource(path));
    }

    JsonResource* ResourceManager::GetJsonResourceFileName(const std::wstring& file_name)
    {
        for (auto& json : m_json_resource_map)
        {
            auto resource = json.second;
            if (resource->FileName() + resource->FileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetJsonResources(const std::wstring& file_name, std::vector<JsonResource*>& resources)
    {
        for (auto& json : m_json_resource_map)
        {
            auto resource = json.second;
            if (resource->FileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->FileName() + resource->FileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    void ResourceManager::QueryFilePathRecursive(const std::wstring& path, std::list<std::wstring>& file_list, std::list<std::wstring>& directory_list) const
    {
        std::list<std::wstring> paths;
        m_file_utility->GetFileListsInPath(path, paths);
        for (auto file_path : paths)
        {
            if (m_file_utility->IsDirectory(file_path))
            {
                directory_list.push_back(file_path);
                QueryFilePathRecursive(file_path, file_list, directory_list);
            }
            else
            {
                file_list.push_back(file_path);
            }
        }
    }

    Resource* ResourceManager::AddResource(const std::wstring& path)
    {
        Resource*    resource;
        std::wstring type = m_file_utility->GetFileTypeFromPath(path);
        std::wstring name = m_file_utility->GetFileNameFromPath(path);
        if (type == L".ps" || type == L".vs")
        {
            resource              = new ShaderResource(path);
            resource->m_file_name = name;
            resource->m_file_type = type;
            m_shader_resource_map.emplace(path, (ShaderResource*)resource);
        }
        else if (type == L".dds" || type == L".DDS"
            || type == L".png" || type == L".PNG"
            || type == L".JPEG" || type == L".jpeg"
            || type == L".JPG" || type == L".jpg"
            || type == L".BMP" || type == L".bmp"
            || type == L".TIFF" || type == L".tiff"
            || type == L".GIF" || type == L".gif")
        {
            resource              = new TextureResource(path);
            resource->m_file_name = name;
            resource->m_file_type = type;
            m_texture_resource_map.emplace(path, (TextureResource*)resource);
        }
        else if (type == L".jsonmdl" || type == L".txtmdl")
        {
            resource              = new MeshResource(path);
            resource->m_file_name = name;
            resource->m_file_type = type;
            m_mesh_resource_map.emplace(path, (MeshResource*)resource);
        }
        else if (type == L".wav")
        {
            resource              = new AudioResource(path);
            resource->m_file_name = name;
            resource->m_file_type = type;
            m_audio_resource_map.emplace(path, (AudioResource*)resource);
        }
        else if (type == L".json")
        {
            resource              = new JsonResource(path);
            resource->m_file_name = name;
            resource->m_file_type = type;
            m_json_resource_map.emplace(path, (JsonResource*)resource);
        }
        else
        {
            resource              = new Resource(path);
            resource->m_file_name = name;
            resource->m_file_type = type;
            m_other_resource_map.emplace(path, resource);
        }
        return resource;
    }
}

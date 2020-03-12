#pragma once
#include <string>
#include <list>
#include <unordered_map>

namespace Engine5
{
    class ShaderResource;
    class TextureResource;
    class MeshResource;
    class AudioResource;
    class Resource;
    class FileUtility;

    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager();

        void Initialize(FileUtility* file_utility);
        void Shutdown();

        Resource* GetResource(const std::wstring& path);
        void      RemoveResource(const std::wstring& path);
        void      RemoveResource(Resource* resource);
        void      ClearResource();
        void      BuildResource();

        //get specific resource
        ShaderResource*  GetShaderResource(const std::wstring& path);
        TextureResource* GetTextureResource(const std::wstring& path);
        MeshResource*    GetMeshResource(const std::wstring& path);
        AudioResource*   GetAudioResource(const std::wstring& path);

        std::unordered_map<std::wstring, ShaderResource*>*  GetShaderResourceMap();
        std::unordered_map<std::wstring, TextureResource*>* GetTextureResourceMap();
        std::unordered_map<std::wstring, MeshResource*>*    GetMeshResourceMap();
        std::unordered_map<std::wstring, AudioResource*>*   GetAudioResourceMap();

    private:
        void      QueryFilePathRecursive(const std::wstring& path, std::list<std::wstring>& file_list, std::list<std::wstring>& directory_list) const;
        Resource* AddResource(const std::wstring& path);

    private:
        std::list<std::wstring> m_file_list;
        std::list<std::wstring> m_directory_list;

        std::unordered_map<std::wstring, Resource*>        m_resource_map;
        std::unordered_map<std::wstring, ShaderResource*>  m_shader_resource_map;
        std::unordered_map<std::wstring, TextureResource*> m_texture_resource_map;
        std::unordered_map<std::wstring, MeshResource*>    m_mesh_resource_map;
        std::unordered_map<std::wstring, AudioResource*>   m_audio_resource_map;

        FileUtility* m_file_utility = nullptr;
    private:
    };
}

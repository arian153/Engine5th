#pragma once
#include "../Resource.hpp"
#include "../../../System/Graphics/DataType/MeshData.hpp"

namespace Engine5
{
    enum class eMeshType : size_t
    {
        Invalid
      , CustomTXT
      , WaveFrontOBJ
    };

    class MeshResource final : public Resource
    {
    public:
        explicit MeshResource(const std::wstring& path);
        ~MeshResource();

        void Initialize() override;
        void Shutdown() override;

        void Load();

        void LoadWaveFrontOBJ();
        void LoadCustomTXT();
        void CheckMeshType();

    private:
        eMeshType m_mesh_type = eMeshType::Invalid;
        MeshData  m_mesh_data;
    };
}

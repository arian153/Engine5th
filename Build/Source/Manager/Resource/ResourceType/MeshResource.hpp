#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class MeshResource final : public Resource
    {
    public:
        explicit MeshResource(const std::wstring& path);
        ~MeshResource();

        void Initialize() override;
        void Shutdown() override;

    private:
    };
}

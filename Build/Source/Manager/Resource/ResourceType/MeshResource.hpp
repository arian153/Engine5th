#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class MeshResource : public Resource
    {
    public:
        explicit MeshResource(const std::wstring& path);
        ~MeshResource();

    private:
    };
}

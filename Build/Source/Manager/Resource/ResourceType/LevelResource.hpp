#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class LevelResource : public Resource
    {
    public:
        explicit LevelResource(const std::wstring& path);
        ~LevelResource();

    private:
    };
}

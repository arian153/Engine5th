#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class LevelResource : public Resource
    {
    public:
        LevelResource(const std::wstring& path);
        ~LevelResource();

    private:
    };
}

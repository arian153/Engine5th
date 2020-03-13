#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class JsonResource final : public Resource
    {
    public:
        explicit JsonResource(const std::wstring& path);
        ~JsonResource();

    private:


    };
}

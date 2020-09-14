#pragma once

#include "../Resource.hpp"

namespace Engine5
{
    class UndefinedResource final : public Resource
    {
    public:
        explicit UndefinedResource(const std::wstring& path);
        ~UndefinedResource();

        void Initialize() override;
        void Shutdown() override;

    private:
    };
}

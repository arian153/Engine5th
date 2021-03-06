#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class AudioResource final : public Resource
    {
    public:
        explicit AudioResource(const std::wstring& path);
        ~AudioResource();

        void Initialize() override;
        void Shutdown() override;

    private:
    };
}

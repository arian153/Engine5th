#include "AudioResource.hpp"

namespace Engine5
{
    AudioResource::AudioResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Audio;
    }

    AudioResource::~AudioResource()
    {
    }
}

#include "JsonResource.hpp"

namespace Engine5
{
    JsonResource::JsonResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Json;
    }

    JsonResource::~JsonResource()
    {
    }
}

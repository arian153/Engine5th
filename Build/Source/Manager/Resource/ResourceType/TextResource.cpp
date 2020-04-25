#include "TextResource.hpp"

namespace Engine5
{
    TextResource::TextResource(const std::wstring& path)
        : Resource(path)
    {
    }

    TextResource::~TextResource()
    {
    }

    void TextResource::Initialize()
    {
        if (m_text == nullptr)
        {
            m_text     = new std::wstring();
            m_b_loaded = true;
        }
    }

    void TextResource::Shutdown()
    {
        if (m_text != nullptr)
        {
            delete m_text;
            m_text       = nullptr;
            m_b_unloaded = true;
        }
    }

    std::wstring* TextResource::GetText() const
    {
        return m_text;
    }
}

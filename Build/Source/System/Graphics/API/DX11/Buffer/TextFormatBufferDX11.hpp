#pragma once
#include <dwrite_2.h>
#include <d2d1_2.h>
#include <string>

namespace Engine5
{
    class TextFormatBufferDX11
    {
    public:
        TextFormatBufferDX11();
        ~TextFormatBufferDX11();

        bool BuildTextFormats(const std::wstring& font);

        IDWriteTextFormat* GetTextFormat() const;

    protected:
        IDWriteFactory2* m_write_factory = nullptr;
        IDWriteTextFormat* m_text_format = nullptr;
    };
}

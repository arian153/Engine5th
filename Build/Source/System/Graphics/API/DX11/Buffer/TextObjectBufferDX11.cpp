#include "TextObjectBufferDX11.hpp"
#include "../../../Buffer/TextObjectBufferCommon.hpp"

namespace Engine5
{
    TextObjectBufferDX11::TextObjectBufferDX11()
    {
    }

    TextObjectBufferDX11::~TextObjectBufferDX11()
    {
    }

    bool TextObjectBufferDX11::SetUpTextFormats()
    {
        HRESULT result = m_write_factory->CreateTextFormat(
                                                           L"courier new",
                                                           nullptr,
                                                           DWRITE_FONT_WEIGHT_NORMAL,
                                                           DWRITE_FONT_STYLE_NORMAL,
                                                           DWRITE_FONT_STRETCH_NORMAL,
                                                           14.0f,
                                                           L"en-GB",
                                                           &m_text_format
                                                          );
        if (FAILED(result))
        {
            return false;
        }
        result = m_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        if (FAILED(result))
        {
            return false;
        }
        result = m_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    bool TextObjectBufferDX11::SetUpTextLayout()
    {

        HRESULT result = m_write_factory->CreateTextLayout(
            text.c_str(),      // The string to be laid out and formatted.
            (UINT32)text.length(),  // The length of the string.
            m_text_format,  // The text format to apply to the string (contains font information, etc).
            (Real)width,         // The width of the layout box.
            (Real)height,        // The height of the layout box.
            &m_text_layout  // The IDWriteTextLayout interface pointer.
        );

        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    TextObjectBufferCommon::TextObjectBufferCommon()
    {
    }

    TextObjectBufferCommon::~TextObjectBufferCommon()
    {
    }
}

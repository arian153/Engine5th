#pragma once
#include <dwrite_2.h>
#include <d2d1_2.h>
#include <string>
#include "../../../../Math/Algebra/Vector2.hpp"
#include "../../../DataType/Color.hpp"

namespace Engine5
{
    class TextFormatBufferCommon;

    class TextLayoutBufferDX11
    {
    public:
        TextLayoutBufferDX11();
        ~TextLayoutBufferDX11();

        bool SetTextLayout(const std::wstring& text, const Vector2& size);
        void SetTextFormat(TextFormatBufferCommon* text_format);

    protected:
        IDWriteFactory2*      m_write_factory      = nullptr;
        ID2D1DeviceContext1*  m_d2d_device_context = nullptr;
        IDWriteTextFormat*    m_text_format        = nullptr;
        IDWriteTextLayout*    m_text_layout        = nullptr;
        ID2D1SolidColorBrush* m_brush              = nullptr;
        Vector2               m_position;
    };
}

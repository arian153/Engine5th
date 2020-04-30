#pragma once
#include <d3d11.h>
#include <dwrite_2.h>
#include <d2d1_2.h>
#include <string>
#include "../../../../Math/Algebra/Vector2.hpp"
#include "../../../DataType/Color.hpp"

namespace Engine5
{
    class TextObjectBufferDX11
    {
    public:
        TextObjectBufferDX11();
        ~TextObjectBufferDX11();

        bool SetUpTextFormats();
        bool SetUpTextLayout();

    private:
        IDWriteFactory2* m_write_factory = nullptr;
        IDWriteTextFormat*    m_text_format = nullptr;
        IDWriteTextLayout*    m_text_layout = nullptr;
        ID2D1SolidColorBrush* m_brush       = nullptr;
        Vector2               m_position;
        Color                 m_color;

        
    };
}

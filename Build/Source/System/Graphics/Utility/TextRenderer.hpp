#pragma once
#include "../../Math/Math.hpp"
#include "../Buffer/TextLayoutBufferCommon.hpp"

namespace Engine5
{
    class MatrixManager;
    class TextFormatBufferCommon;
    class RendererCommon;

    class TextRenderer
    {
    public:
        TextRenderer();
        ~TextRenderer();

        void Initialize(RendererCommon* renderer, MatrixManager* matrix_manager);
        void Render();
        void Shutdown();
        void ClearTextLayouts();

        void Print(const Vector2& position, const Color& color, const std::wstring& text);

    private:
        RendererCommon* m_renderer = nullptr;
        MatrixManager* m_matrix_manager = nullptr;
        TextFormatBufferCommon* m_text_format = nullptr;

        std::vector<TextLayoutBufferCommon> m_text_layouts;
    };
}

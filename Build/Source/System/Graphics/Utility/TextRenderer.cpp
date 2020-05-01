#include "TextRenderer.hpp"
#include "../Buffer/TextFormatBufferCommon.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "MatrixManager.hpp"

namespace Engine5
{
    TextRenderer::TextRenderer()
    {
    }

    TextRenderer::~TextRenderer()
    {
    }

    void TextRenderer::Initialize(RendererCommon* renderer, MatrixManager* matrix_manager)
    {
        m_renderer           = renderer;
        m_matrix_manager     = matrix_manager;
        m_text_format = new TextFormatBufferCommon();
        m_text_format->Initialize(renderer);
        m_text_format->BuildTextFormats(L"courier new", 14.0f);
    }

    void TextRenderer::Render()
    {
        if (!m_text_layouts.empty())
        {
            m_renderer->SetUpDWRenderTarget();
            for (auto& layout : m_text_layouts)
            {
                layout.Render();
            }
            ClearTextLayouts();
        }
    }

    void TextRenderer::Shutdown()
    {
        ClearTextLayouts();
        if (m_text_format != nullptr)
        {
            m_text_format->Shutdown();
            delete m_text_format;
            m_text_format = nullptr;
        }
    }

    void TextRenderer::ClearTextLayouts()
    {
        for (auto& layout : m_text_layouts)
        {
            layout.Shutdown();
        }
        m_text_layouts.clear();
    }

    void TextRenderer::Print(const Vector2& position, const Color& color, const std::wstring& text)
    {
        auto text_layout = m_text_layouts.emplace_back();
        text_layout.Initialize(m_renderer);
        text_layout.SetTextFormat(m_text_format);
        text_layout.SetPosition(position);
        text_layout.SetColor(color);
        text_layout.SetTextLayout(text, m_matrix_manager->GetScreenWidth(), m_matrix_manager->GetScreenHeight());
    }
}

#include "RenderTextureGenerator.hpp"
#include "../Buffer/RenderTextureBufferCommon.hpp"
#include "MatrixManager.hpp"
#include "../Texture/TextureCommon.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "../Element/Scene.hpp"
#include "../DataType/MatrixData.hpp"
#include "../Shader/ShaderManagerCommon.hpp"
#include "../Element/TextSprite.hpp"

namespace Engine5
{
    RenderTextureGenerator::RenderTextureGenerator()
    {
    }

    RenderTextureGenerator::~RenderTextureGenerator()
    {
    }

    void RenderTextureGenerator::Initialize(RendererCommon* renderer, MatrixManager* matrix_manager, ShaderManagerCommon* shader_manager)
    {
        m_renderer       = renderer;
        m_matrix_manager = matrix_manager;
        m_shader_manager = shader_manager;
        m_width          = m_matrix_manager->GetScreenWidth();
        m_height         = m_matrix_manager->GetScreenHeight();
        m_render_texture = new RenderTextureBufferCommon();
        m_render_texture->Initialize(renderer, m_width, m_height);
        m_texture = new TextureCommon();
    }

    void RenderTextureGenerator::Shutdown()
    {
        if (m_render_texture != nullptr)
        {
            m_render_texture->Shutdown();
            delete m_render_texture;
            m_render_texture = nullptr;
        }
        if (m_texture != nullptr)
        {
            delete m_texture;
            m_texture = nullptr;
        }
    }

    void RenderTextureGenerator::Render(Scene* scene)
    {
        scene->Update(0.0f);
        scene->Render();
    }

    void RenderTextureGenerator::Render(Mesh* mesh, Camera* camera, Light* light) const
    {
        MatrixData mvp_data(
                            mesh->GetModelMatrix(),
                            camera->GetViewMatrix(),
                            m_matrix_manager->GetPerspectiveMatrix()
                           );
        auto type = mesh->GetShaderType();
        if (type == eShaderType::Color)
        {
            mesh->RenderBuffer();
            m_shader_manager->RenderColorShader(mesh->GetIndexCount(), mvp_data);
        }
        else if (type == eShaderType::Texture)
        {
            mesh->RenderBuffer();
            m_shader_manager->RenderTextureShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), mesh->GetColor());
        }
        else if (type == eShaderType::ForwardDirectionalLight)
        {
            mesh->RenderBuffer();
            m_shader_manager->RenderForwardDirectionalLightShader(
                                                                  mesh->GetIndexCount(),
                                                                  mvp_data,
                                                                  mesh->GetTexture(),
                                                                  camera,
                                                                  mesh->GetColor(),
                                                                  (DirectionalLight*)light
                                                                 );
        }
    }

    void RenderTextureGenerator::Render(Sprite* sprite, Camera* camera)
    {
        //todo : Sprite has not been implemented yet
    }

    void RenderTextureGenerator::Render(ParticleEmitter* particle_emitter, Camera* camera)
    {
        //todo : ParticleEmitter has not been implemented yet
    }

    void RenderTextureGenerator::Render(TextSprite* text_sprite, Camera* camera) const
    {
        MatrixData mvp_data(
                            text_sprite->GetModelMatrix(),
                            camera->GetViewMatrix(),
                            m_matrix_manager->GetPerspectiveMatrix()
                           );
        text_sprite->Render();
        m_shader_manager->RenderTextureShader(
                                              text_sprite->GetIndexCount(),
                                              mvp_data,
                                              text_sprite->GetTexture(),
                                              text_sprite->GetColor());
    }

    void RenderTextureGenerator::BeginRenderToTexture(const Color& color) const
    {
        m_render_texture->SetRenderTarget();
        m_render_texture->ClearRenderTarget(color);
    }

    void RenderTextureGenerator::EndRenderToTexture() const
    {
        m_renderer->SetBackBufferRenderTarget();
    }

    void RenderTextureGenerator::SetTextureSize(size_t width, size_t height)
    {
        m_width  = width;
        m_height = height;
        ResizeTexture();
    }

    void RenderTextureGenerator::SetTextureScreenSize()
    {
        m_b_sync_screen = true;
        m_width         = m_matrix_manager->GetScreenWidth();
        m_height        = m_matrix_manager->GetScreenHeight();
        ResizeTexture();
    }

    void RenderTextureGenerator::OnResize()
    {
        if (m_b_sync_screen == true)
        {
            m_width  = m_matrix_manager->GetScreenWidth();
            m_height = m_matrix_manager->GetScreenHeight();
            ResizeTexture();
        }
    }

    void RenderTextureGenerator::DrawRenderToTextureBuffer(const Color& color) const
    {
        MatrixData mvp_data;
        mvp_data.projection = m_matrix_manager->GetOrthoGraphicMatrix();
        m_render_texture->Render();
        m_shader_manager->RenderTextureShader(
                                              m_render_texture->GetIndexCount(),
                                              mvp_data,
                                              GetTexture(),
                                              color
                                             );
    }

    TextureCommon* RenderTextureGenerator::GetTexture() const
    {
        m_texture->Initialize(m_render_texture);
        return m_texture;
    }

    void RenderTextureGenerator::ResizeTexture() const
    {
        m_render_texture->OnResize(m_width, m_height);
    }
}

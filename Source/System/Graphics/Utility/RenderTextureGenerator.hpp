#pragma once

namespace Engine5
{
    class Space;
    class ShaderManagerCommon;
    class Camera;
    class MatrixManager;
    class RenderTextureBufferCommon;
    class Color;
    class RendererCommon;
    class TextureCommon;
    class Light;
    class Mesh;
    class Mirror;
    class Sprite;
    class Water;
    class TextSprite;
    class ParticleEmitter;
    class Scene;

    class RenderTextureGenerator
    {
    public:
        RenderTextureGenerator();
        ~RenderTextureGenerator();

        void Initialize(RendererCommon* renderer, MatrixManager* matrix_manager, ShaderManagerCommon* shader_manager);
        void Shutdown();

        //render elements
        void Render(Space* space);
        void Render(Scene* scene);
        void Render(Mesh* mesh, Camera* camera, Light* light) const;
        void Render(Sprite* sprite, Camera* camera);
        void Render(ParticleEmitter* particle_emitter, Camera* camera);
        void Render(TextSprite* text_sprite, Camera* camera) const;

        void BeginRenderToTexture(const Color& color) const;
        void EndRenderToTexture() const;
        void SetTextureSize(size_t width, size_t height);
        void SetTextureScreenSize();
        void OnResize();
        void DrawRenderToTextureBuffer(const Color& color) const;

        TextureCommon* GetTexture() const;

    private :
        void ResizeTexture() const;

    private:
        RenderTextureBufferCommon* m_render_texture = nullptr;
        TextureCommon*             m_texture        = nullptr;
        RendererCommon*            m_renderer       = nullptr;
        MatrixManager*             m_matrix_manager = nullptr;
        ShaderManagerCommon*       m_shader_manager = nullptr;
        size_t                     m_width          = 0;
        size_t                     m_height         = 0;
        bool                       m_b_sync_screen  = false;
    };
}

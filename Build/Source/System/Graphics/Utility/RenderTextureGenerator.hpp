#pragma once

namespace Engine5
{
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

        void Initialize(RendererCommon* renderer, MatrixManager* matrix_manager);
        void Shutdown();

        //render elements 
        void Render(Scene* scene);
        void Render(Mesh* mesh);
        void Render(Light* light);
        void Render(Sprite* sprite);
        void Render(ParticleEmitter* particle_emitter);
        void Render(TextSprite* text_sprite);

        void BeginRenderToTexture(const Color& color) const;
        void EndRenderToTexture() const;
        void SetTextureSize(size_t width, size_t height);
        void SetTextureScreenSize();
        void OnResize();

        TextureCommon* GetTexture() const;

    private :
        void ResizeTexture() const;

    private:
        RenderTextureBufferCommon* m_render_texture = nullptr;
        TextureCommon*             m_texture        = nullptr;
        RendererCommon*            m_renderer       = nullptr;
        MatrixManager*             m_matrix_manager = nullptr;
        size_t                     m_width          = 0;
        size_t                     m_height         = 0;
        bool                       m_b_sync_screen  = false;
    };
}

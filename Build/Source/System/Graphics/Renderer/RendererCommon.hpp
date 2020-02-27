#pragma once
#include "DX11/RendererDX11.hpp"
#include "APIMacro.hpp"

namespace Engine5
{

    class RendererCommon : public RendererAPI
    {
    public:
        RendererCommon();
        ~RendererCommon();

        void Initialize(int client_width, int client_height, bool b_fullscreen, Real far_plane, Real near_plane, Real field_of_view);
        void Shutdown();
        void OnResize(int client_width, int client_height, bool b_fullscreen, Real far_plane, Real near_plane, Real field_of_view);
        void OnFullscreen(bool b_fullscreen) const;

        void Render() const;

        void BeginScene(Color color) const;
        void EndScene() const;
        void SetVSync(bool flag);
        void SetAlphaBlending(bool flag) const;
        void SetZBuffering(bool flag) const;
        void SetMatrixGenerator(MatrixGenerator* matrix_generator);
        
    private:
    };
}

#pragma once

#define E5_DIRECTX11

#if defined(E5_DIRECTX11)
#define RendererAPI RendererDX11
#elif defined(E5_DIRECTX12)
#define RendererAPI RendererDX12
#elif defined(E5_OPENGL4)
#define RendererAPI RendererOpenGL4
#endif


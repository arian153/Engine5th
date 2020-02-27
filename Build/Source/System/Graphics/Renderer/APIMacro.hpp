#pragma once

#define E5_DIRECTX11

#if defined(E5_DIRECTX11)
#define RendererAPI RendererDX11
#elif defined(E5_DIRECTX12)
#define RendererAPI RendererDX12
#elif defined(E5_OPENGL4)
#define RendererAPI RendererOpenGL4
#endif


#if defined(E5_DIRECTX11)
#define BufferAPI BufferDX11
#elif defined(E5_DIRECTX12)
#define BufferAPI BufferDX12
#elif defined(E5_OPENGL4)
#define BufferAPI BufferOpenGL4
#endif


#if defined(E5_DIRECTX11)
#define ShaderAPI ShaderDX11
#elif defined(E5_DIRECTX12)
#define ShaderAPI ShaderDX12
#elif defined(E5_OPENGL4)
#define ShaderAPI ShaderOpenGL4
#endif



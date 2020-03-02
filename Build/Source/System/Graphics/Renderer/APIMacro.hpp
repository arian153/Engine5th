#pragma once
#define E5_DIRECTX11

//Renderer
#if defined(E5_DIRECTX11)
#define RendererAPI RendererDX11
#elif defined(E5_DIRECTX12)
#define RendererAPI RendererDX12
#elif defined(E5_OPENGL4)
#define RendererAPI RendererOpenGL4
#endif

//Buffer
#if defined(E5_DIRECTX11)
#define BufferAPI BufferDX11
#elif defined(E5_DIRECTX12)
#define BufferAPI BufferDX12
#elif defined(E5_OPENGL4)
#define BufferAPI BufferOpenGL4
#endif

//ColorVertex
#if defined(E5_DIRECTX11)
#define ColorVertexAPI ColorVertexDX11
#elif defined(E5_DIRECTX12)
#define ColorVertexAPI ColorVertexDX12
#elif defined(E5_OPENGL4)
#define ColorVertexAPI ColorVertexOpenGL4
#endif

//TextureVertex
#if defined(E5_DIRECTX11)
#define TextureVertexAPI TextureVertexDX11
#elif defined(E5_DIRECTX12)
#define TextureVertexAPI TextureVertexDX12
#elif defined(E5_OPENGL4)
#define TextureVertexAPI TextureVertexOpenGL4
#endif

//Shader Manager
#if defined(E5_DIRECTX11)
#define ShaderManagerAPI ShaderManagerDX11
#elif defined(E5_DIRECTX12)
#define ShaderManagerAPI ShaderManagerDX12
#elif defined(E5_OPENGL4)
#define ShaderManagerAPI ShaderManagerOpenGL4
#endif

//Color Shader
#if defined(E5_DIRECTX11)
#define ColorShaderAPI ColorShaderDX11
#elif defined(E5_DIRECTX12)
#define ColorShaderAPI ColorShaderDX12
#elif defined(E5_OPENGL4)
#define ColorShaderAPI ColorShaderOpenGL4
#endif

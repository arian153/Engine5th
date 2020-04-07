#pragma once

#if defined(E5_DIRECTX11)
//classes
#define RendererAPI RendererDX11
#define MeshBufferAPI MeshBufferDX11
#define DeferredBufferAPI DeferredBufferDX11
#define TextureAPI TextureDX11
#define ColorVertexAPI ColorVertexDX11
#define TextureVertexAPI TextureVertexDX11
#define ShaderManagerAPI ShaderManagerDX11
#define ColorShaderAPI ColorShaderDX11
#define TextureShaderAPI TextureShaderDX11
#define LightShaderAPI LightShaderDX11

//includes
#define IncludeRendererAPI "../API/DX11/RendererDX11.hpp"
#define IncludeMeshBufferAPI "../API/DX11/Buffer/MeshBufferDX11.hpp"
#define IncludeDeferredBufferAPI "../API/DX11/Buffer/DeferredBufferDX11.hpp"
#define IncludeTextureAPI "../API/DX11/Texture/TextureDX11.hpp"
#define IncludeColorVertexAPI "../API/DX11/Vertex/ColorVertexDX11.hpp"
#define IncludeTextureVertexAPI "../API/DX11/Vertex/TextureVertexDX11.hpp"
#define IncludeShaderManagerAPI "../API/DX11/Shader/ShaderManagerDX11.hpp"
#define IncludeColorShaderAPI "../API/DX11/Shader/ColorShaderDX11.hpp"
#define IncludeTextureShaderAPI "../API/DX11/Shader/TextureShaderDX11.hpp"
#define IncludeLightShaderAPI "../API/DX11/Shader/LightShaderDX11.hpp"

#elif defined(E5_DIRECTX12)
//classes
#define RendererAPI RendererDX12
#define BufferAPI BufferDX12
#define TextureAPI TextureDX12
#define ColorVertexAPI ColorVertexDX12
#define TextureVertexAPI TextureVertexDX12
#define ShaderManagerAPI ShaderManagerDX12
#define ColorShaderAPI ColorShaderDX12

//includes
#define IncludeRendererAPI "DX12/RendererDX12.hpp"
#define IncludeBufferAPI "../Renderer/DX12/BufferDX12.hpp"
#define IncludeTextureAPI "../Renderer/DX12/TextureDX12.hpp"
#define IncludeColorVertexAPI "../../Renderer/DX12/Vertex/ColorVertexDX12.hpp"
#define IncludeTextureVertexAPI "../../Renderer/DX12/Vertex/TextureVertexDX12.hpp"
#define IncludeShaderManagerAPI "../Renderer/DX12/Shader/ShaderManagerDX12.hpp"
#define IncludeColorShaderAPI "../Renderer/DX12/Shader/ColorShaderDX12.hpp"

#elif defined(E5_OPENGL4)
//classes
#define RendererAPI RendererOpenGL4
#define BufferAPI BufferOpenGL4
#define TextureAPI TextureOpenGL4
#define ColorVertexAPI ColorVertexOpenGL4
#define TextureVertexAPI TextureVertexOpenGL4
#define ShaderManagerAPI ShaderManagerOpenGL4
#define ColorShaderAPI ColorShaderOpenGL4

//includes
#define IncludeRendererAPI "OpenGL4/RendererOpenGL4.hpp"
#define IncludeBufferAPI "../Renderer/OpenGL4/BufferOpenGL4.hpp"
#define IncludeTextureAPI "../Renderer/OpenGL4/TextureOpenGL4.hpp"
#define IncludeColorVertexAPI "../../Renderer/OpenGL4/Vertex/ColorVertexOpenGL4.hpp"
#define IncludeTextureVertexAPI "../../Renderer/OpenGL4/Vertex/TextureVertexOpenGL4.hpp"
#define IncludeShaderManagerAPI "../Renderer/OpenGL4/Shader/ShaderManagerOpenGL4.hpp"
#define IncludeColorShaderAPI "../Renderer/OpenGL4/Shader/ColorShaderOpenGL4.hpp"
#endif

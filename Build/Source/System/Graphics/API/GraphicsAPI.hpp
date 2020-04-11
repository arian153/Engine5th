#pragma once

#if defined(E5_DIRECTX11)
//classes
#define RendererAPI RendererDX11
//buffer
#define MeshBufferAPI MeshBufferDX11
#define DeferredBufferAPI DeferredBufferDX11
#define RenderTextureBufferAPI RenderTextureBufferDX11

//others
#define TextureAPI TextureDX11
#define ColorVertexAPI ColorVertexDX11
#define NormalVertexAPI NormalVertexDX11
#define TextureVertexAPI TextureVertexDX11

//shader
#define ShaderManagerAPI ShaderManagerDX11
#define ColorShaderAPI ColorShaderDX11
#define TextureShaderAPI TextureShaderDX11
#define LightShaderAPI LightShaderDX11
#define DeferredShaderAPI DeferredShaderDX11

//includes
#define IncludeRendererAPI "../API/DX11/RendererDX11.hpp"
#define IncludeMeshBufferAPI "../API/DX11/Buffer/MeshBufferDX11.hpp"
#define IncludeDeferredBufferAPI "../API/DX11/Buffer/DeferredBufferDX11.hpp"
#define IncludeRenderTextureBufferAPI "../API/DX11/Buffer/RenderTextureBufferDX11.hpp"
#define IncludeTextureAPI "../API/DX11/Texture/TextureDX11.hpp"
#define IncludeColorVertexAPI "../API/DX11/Vertex/ColorVertexDX11.hpp"
#define IncludeNormalVertexAPI "../API/DX11/Vertex/NormalVertexDX11.hpp"
#define IncludeTextureVertexAPI "../API/DX11/Vertex/TextureVertexDX11.hpp"
#define IncludeShaderManagerAPI "../API/DX11/Shader/ShaderManagerDX11.hpp"
#define IncludeColorShaderAPI "../API/DX11/Shader/ColorShaderDX11.hpp"
#define IncludeTextureShaderAPI "../API/DX11/Shader/TextureShaderDX11.hpp"
#define IncludeLightShaderAPI "../API/DX11/Shader/LightShaderDX11.hpp"
#define IncludeDeferredShaderAPI "../API/DX11/Shader/DeferredShaderDX11.hpp"

#elif defined(E5_DIRECTX12)
#elif defined(E5_OPENGL4)
#endif

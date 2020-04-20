#pragma once

#if defined(E5_DIRECTX11)
//classes
#define RendererAPI RendererDX11
//buffer
#define MeshBufferAPI MeshBufferDX11
#define DeferredBufferAPI DeferredBufferDX11
#define RenderTextureBufferAPI RenderTextureBufferDX11
#define TextBufferAPI TextBufferDX11

//others
#define TextureAPI TextureDX11
#define ColorVertexAPI ColorVertexDX11
#define NormalVertexAPI NormalVertexDX11
#define TextureVertexAPI TextureVertexDX11

//forward shader
#define ShaderManagerAPI ShaderManagerDX11
#define ColorShaderAPI ColorShaderDX11
#define TextureShaderAPI TextureShaderDX11
#define ForwardDirectionalLightShaderAPI ForwardDirectionalLightShaderDX11

//deferred shader
#define DeferredDirectionalLightShaderAPI DeferredDirectionalLightShaderDX11
#define DeferredBufferShaderAPI DeferredBufferShaderDX11

//includes
#define IncludeRendererAPI "../API/DX11/RendererDX11.hpp"
#define IncludeMeshBufferAPI "../API/DX11/Buffer/MeshBufferDX11.hpp"
#define IncludeDeferredBufferAPI "../API/DX11/Buffer/DeferredBufferDX11.hpp"
#define IncludeRenderTextureBufferAPI "../API/DX11/Buffer/RenderTextureBufferDX11.hpp"
#define IncludeTextBufferAPI "../API/DX11/Buffer/TextBufferDX11.hpp"
#define IncludeTextureAPI "../API/DX11/Texture/TextureDX11.hpp"
#define IncludeColorVertexAPI "../API/DX11/Vertex/ColorVertexDX11.hpp"
#define IncludeNormalVertexAPI "../API/DX11/Vertex/NormalVertexDX11.hpp"
#define IncludeTextureVertexAPI "../API/DX11/Vertex/TextureVertexDX11.hpp"
#define IncludeShaderManagerAPI "../API/DX11/Shader/ShaderManagerDX11.hpp"

//forward
#define IncludeColorShaderAPI "../../API/DX11/Shader/Forward/ColorShaderDX11.hpp"
#define IncludeTextureShaderAPI "../../API/DX11/Shader/Forward/TextureShaderDX11.hpp"
#define IncludeForwardDirectionalLightShaderAPI "../../API/DX11/Shader/Forward/ForwardDirectionalLightShaderDX11.hpp"

//deferred
#define IncludeDeferredBufferShaderAPI "../../API/DX11/Shader/Deferred/DeferredBufferShaderDX11.hpp"
#define IncludeDeferredDirectionalLightShaderAPI "../../API/DX11/Shader/Deferred/DeferredDirectionalLightShaderDX11.hpp"

#elif defined(E5_DIRECTX12)
#elif defined(E5_OPENGL4)
#endif

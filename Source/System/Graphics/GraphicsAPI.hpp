#pragma once

#if defined(E5_DIRECTX11)
//classes
#define RendererAPI RendererDX11
//buffer
#define MeshBufferAPI MeshBufferDX11
#define DeferredBufferAPI DeferredBufferDX11
#define RenderTextureBufferAPI RenderTextureBufferDX11
#define TextTextureBufferAPI TextTextureBufferDX11
#define TextLayoutBufferAPI TextLayoutBufferDX11
#define TextFormatBufferAPI TextFormatBufferDX11
#define InstanceBufferAPI InstanceBufferDX11
#define DynamicMeshBufferAPI DynamicMeshBufferDX11

//others
#define TextureAPI TextureDX11
#define TextureArrayAPI TextureArrayDX11
#define TextureAtlasAPI TextureAtlasDX11
#define ColorVertexAPI ColorVertexDX11
#define NormalVertexAPI NormalVertexDX11
#define TextureVertexAPI TextureVertexDX11
#define InstanceDataAPI InstanceDataDX11

//forward shader
#define ShaderManagerAPI ShaderManagerDX11
#define ColorShaderAPI ColorShaderDX11
#define TextureShaderAPI TextureShaderDX11
#define InstanceTextureShaderAPI InstanceTextureShaderDX11
#define ForwardDirectionalLightShaderAPI ForwardDirectionalLightShaderDX11
#define MultiTextureShaderAPI MultiTextureShaderDX11
#define AlphaMappingShaderAPI AlphaMappingShaderDX11
#define LightMappingShaderAPI LightMappingShaderDX11
#define NormalMappingShaderAPI NormalMappingShaderDX11
#define SpecularMappingShaderAPI SpecularMappingShaderDX11


//deferred shader
#define DeferredDirectionalLightShaderAPI DeferredDirectionalLightShaderDX11
#define DeferredBufferShaderAPI DeferredBufferShaderDX11

//includes
#define IncludeRendererAPI "../../DX11/RendererDX11.hpp"
#define IncludeMeshBufferAPI "../../DX11/Buffer/MeshBufferDX11.hpp"
#define IncludeDeferredBufferAPI "../../DX11/Buffer/DeferredBufferDX11.hpp"
#define IncludeRenderTextureBufferAPI "../../DX11/Buffer/RenderTextureBufferDX11.hpp"
#define IncludeTextTextureBufferAPI "../../DX11/Buffer/TextTextureBufferDX11.hpp"
#define IncludeTextLayoutBufferAPI "../../DX11/Buffer/TextLayoutBufferDX11.hpp"
#define IncludeTextFormatBufferAPI "../../DX11/Buffer/TextFormatBufferDX11.hpp"
#define IncludeInstanceBufferAPI "../../DX11/Buffer/InstanceBufferDX11.hpp"
#define IncludeDynamicMeshBufferAPI "../../DX11/Buffer/DynamicMeshBufferDX11.hpp"

#define IncludeTextureAPI "../../DX11/Texture/TextureDX11.hpp"
#define IncludeTextureArrayAPI "../../DX11/Texture/TextureArrayDX11.hpp"
#define IncludeTextureAtlasAPI "../../DX11/Texture/TextureAtlasDX11.hpp"
#define IncludeColorVertexAPI "../../DX11/Vertex/ColorVertexDX11.hpp"
#define IncludeNormalVertexAPI "../../DX11/Vertex/NormalVertexDX11.hpp"
#define IncludeTextureVertexAPI "../../DX11/Vertex/TextureVertexDX11.hpp"
#define IncludeShaderManagerAPI "../../DX11/Shader/ShaderManagerDX11.hpp"
#define IncludeInstanceDataAPI "../../DX11/Vertex/InstanceDataDX11.hpp"

//forward
#define IncludeColorShaderAPI "../../../DX11/Shader/Forward/ColorShaderDX11.hpp"
#define IncludeTextureShaderAPI "../../../DX11/Shader/Forward/TextureShaderDX11.hpp"
#define IncludeInstanceTextureShaderAPI "../../../DX11/Shader/Forward/InstanceTextureShaderDX11.hpp"
#define IncludeForwardDirectionalLightShaderAPI "../../../DX11/Shader/Forward/ForwardDirectionalLightShaderDX11.hpp"
#define IncludeMultiTextureShaderAPI "../../../DX11/Shader/Forward/MultiTextureShaderDX11.hpp"
#define IncludeAlphaMappingShaderAPI "../../../DX11/Shader/Forward/AlphaMappingShaderDX11.hpp"
#define IncludeLightMappingShaderAPI "../../../DX11/Shader/Forward/LightMappingShaderDX11.hpp"
#define IncludeNormalMappingShaderAPI "../../../DX11/Shader/Forward/NormalMappingShaderDX11.hpp"
#define IncludeSpecularMappingShaderAPI "../../../DX11/Shader/Forward/SpecularMappingShaderDX11.hpp"

//deferred
#define IncludeDeferredBufferShaderAPI "../../../DX11/Shader/Deferred/DeferredBufferShaderDX11.hpp"
#define IncludeDeferredDirectionalLightShaderAPI "../../../DX11/Shader/Deferred/DeferredDirectionalLightShaderDX11.hpp"

#elif defined(E5_DIRECTX12)
#elif defined(E5_OPENGL4)
#endif

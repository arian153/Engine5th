#pragma once
#include <unordered_map>

#include "../../Math/Utility/MathDef.hpp"
#include "Camera.hpp"
#include "../DataType/ProjectionType.hpp"
#include <vector>

#include "../../../Manager/Component/EngineComponent/MeshComponent.hpp"
#include "../DataType/Frustum.hpp"

namespace Engine5
{
    class Light2;
    class MeshData;
    struct MaterialTexture;
    class MaterialManager;
    class Mesh;
    class VertexLayoutCommon;
    class ConstantBufferCommon;
    class ShaderProgramCommon;
    class Mesh2;
    class TextRenderer;
    class ResourceManager;
    class ParticleEmitter;
    class TextSprite;
    class TextTextureBufferCommon;
    class RenderTextureBufferCommon;
    class DeferredBufferCommon;
    class ColliderSet;
    class PrimitiveRenderer;
    class MatrixManager;
    class ShaderManagerCommon;
    class RendererCommon;
    class RendererDX11;
    class DirectionalLight;
    class PointLight;
    class SpotLight;
    class CapsuleLight;

    using MeshTable = std::unordered_map<size_t, std::unordered_map<size_t, Mesh2*>>;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void Initialize();
        void Update(Real dt);
        void Render() const;
        void Shutdown();

        void SetRenderer(RendererCommon* renderer);
        void SetShaderManager(ShaderManagerCommon* shader_manager);
        void SetMatrixManager(MatrixManager* matrix_manager);
        void SetMainCamera(Camera* camera);
        void SetResourceManager(ResourceManager* resource_manager);
        void SetTextRenderer(TextRenderer* text_renderer);
        void SetMaterialManager(MaterialManager* material_manager);

        void SetProjectionType(eProjectionType projection_type);
        void UpdateView();
        void UpdateProjection();
        void OnResize() const;

        PrimitiveRenderer* GetPrimitiveRenderer() const;

        Mesh2* GetMesh(size_t model_id, size_t material_id);
        Mesh2* AddMesh(const std::string& model_path, const MaterialTexture& material);
        Mesh2* AddMesh(MeshResource* model_resource, const MaterialTexture& material);
        Mesh2* AddMesh(MeshData* model_data, const MaterialTexture& material);
        void   AddMesh(MeshComponent* mesh_compo);
        void   RemoveMesh(MeshComponent* mesh_compo);

        void AddLight(Light2* light);
        void RemoveLight(Light2* light);

        //add
        Camera* AddCamera(Camera* camera);
        void    AddTextSprite(TextSprite* text_sprite);
        void    AddParticleEmitter(ParticleEmitter* particle_emitter);

        //remove
        void RemoveCamera(Camera* camera);
        void RemoveTextSprite(TextSprite* text_sprite);
        void RemoveParticleEmitter(ParticleEmitter* particle_emitter);

        void InitializeTextSprite(TextSprite* text_sprite) const;
        void InitializeParticleEmitter(ParticleEmitter* particle_emitter) const;

        Basis GetMainCameraBasis() const;
        Real  GetAspectRatio() const;
        Ray   GetPickingRay(const Vector2& pos) const;

    private:
        void SetUpMesh(Mesh2* mesh, MeshData* model_data, const MaterialTexture& material, size_t model_id, size_t material_id) const;
        void UpdateMesh(Real dt);

    private:
        RendererCommon*      m_renderer           = nullptr;
        ShaderManagerCommon* m_shader_manager     = nullptr;
        MatrixManager*       m_matrix_manager     = nullptr;
        ResourceManager*     m_resource_manager   = nullptr;
        Camera*              m_main_camera        = nullptr;
        PrimitiveRenderer*   m_primitive_renderer = nullptr;
        TextRenderer*        m_text_renderer      = nullptr;
        MaterialManager*     m_material_manager   = nullptr;

        std::vector<Camera*>          m_cameras;
        std::vector<TextSprite*>      m_text_sprites;
        std::vector<ParticleEmitter*> m_particle_emitters;
        std::vector<MeshComponent*>   m_mesh_components;
        std::vector<Mesh2*>           m_meshes;
        MeshTable                     m_mesh_table;

        std::vector<Light2*> m_lights;

        eProjectionType m_projection_type = eProjectionType::Perspective;
        Matrix44        m_view_matrix;
        Matrix44        m_projection_matrix;
        Matrix44        m_orthogonal_matrix;
        Frustum         m_frustum;

        bool m_b_deferred_shading = false;

        ConstantBufferCommon* m_matrix_buffer            = nullptr;
        ConstantBufferCommon* m_matrix_instancing_buffer = nullptr;
    };
}

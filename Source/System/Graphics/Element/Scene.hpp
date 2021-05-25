#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "Camera.hpp"
#include "../DataType/ProjectionType.hpp"
#include <vector>
#include "../DataType/Frustum.hpp"

namespace Engine5
{
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

        void SetProjectionType(eProjectionType projection_type);
        void UpdateView();
        void UpdateProjection();
        void OnResize() const;

        PrimitiveRenderer* GetPrimitiveRenderer() const;

        //add
        Camera* AddCamera(Camera* camera);
        Mesh*   AddMesh(Mesh* mesh);
        void    AddLight(DirectionalLight* light);
        void    AddLight(PointLight* light);
        void    AddLight(SpotLight* light);
        void    AddLight(CapsuleLight* light);
        void    AddTextSprite(TextSprite* text_sprite);
        void    AddParticleEmitter(ParticleEmitter* particle_emitter);

        //remove
        void RemoveCamera(Camera* camera);
        void RemoveMesh(Mesh* mesh);
        void RemoveLight(DirectionalLight* light);
        void RemoveLight(PointLight* light);
        void RemoveLight(SpotLight* light);
        void RemoveLight(CapsuleLight* light);
        void RemoveTextSprite(TextSprite* text_sprite);
        void RemoveParticleEmitter(ParticleEmitter* particle_emitter);

        void ChangeShaderType(Mesh* mesh);
        void InitializeTextSprite(TextSprite* text_sprite) const;
        void InitializeParticleEmitter(ParticleEmitter* particle_emitter) const;

        Basis GetMainCameraBasis() const;
        Real  GetAspectRatio() const;
        Ray   GetPickingRay(const Vector2& pos) const;

    private:
        RendererCommon*       m_renderer           = nullptr;
        ShaderManagerCommon*  m_shader_manager     = nullptr;
        MatrixManager*        m_matrix_manager     = nullptr;
        ResourceManager*      m_resource_manager   = nullptr;
        Camera*               m_main_camera        = nullptr;
        PrimitiveRenderer*    m_primitive_renderer = nullptr;
        DeferredBufferCommon* m_deferred_buffer    = nullptr;
        TextRenderer*         m_text_renderer      = nullptr;

        std::vector<Camera*>           m_cameras;
        std::vector<Mesh*>             m_other_meshes;
        std::vector<Mesh*>             m_forward_meshes;
        std::vector<Mesh*>             m_deferred_meshes;
        std::vector<DirectionalLight*> m_directional_lights;
        std::vector<PointLight*>       m_point_lights;
        std::vector<SpotLight*>        m_spot_lights;
        std::vector<CapsuleLight*>     m_capsule_lights;
        std::vector<TextSprite*>       m_text_sprites;
        std::vector<ParticleEmitter*>  m_particle_emitters;

        eProjectionType m_projection_type = eProjectionType::Perspective;
        Matrix44        m_view_matrix;
        Matrix44        m_projection_matrix;
        Matrix44        m_orthogonal_matrix;
        Frustum         m_frustum;

        bool m_b_deferred_shading = false;

        Mesh2*                m_test_mesh           = nullptr;
        ShaderProgramCommon*  m_new_color_shader    = nullptr;
        ConstantBufferCommon* m_matrix_buffer       = nullptr;
        VertexLayoutCommon*   m_color_vertex_layout = nullptr;
    };
}

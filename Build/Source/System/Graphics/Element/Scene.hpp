#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "../DataType/ProjectionType.hpp"
#include <vector>

namespace Engine5
{
    class TextBufferCommon;
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

        //remove
        void RemoveCamera(Camera* camera);
        void RemoveMesh(Mesh* mesh);
        void RemoveLight(DirectionalLight* light);
        void RemoveLight(PointLight* light);
        void RemoveLight(SpotLight* light);
        void RemoveLight(CapsuleLight* light);

        void ChangeShaderType(Mesh* mesh);
    private:
        RendererCommon*      m_renderer           = nullptr;
        ShaderManagerCommon* m_shader_manager     = nullptr;
        MatrixManager*       m_matrix_manager     = nullptr;
        Camera*              m_main_camera        = nullptr;
        PrimitiveRenderer*   m_primitive_renderer = nullptr;
        DeferredBufferCommon*      m_deferred_buffer       = nullptr;

        std::vector<Camera*>           m_cameras;
        std::vector<Mesh*>             m_forward_meshes;
        std::vector<Mesh*>             m_deferred_meshes;
        std::vector<DirectionalLight*> m_directional_lights;
        std::vector<PointLight*>       m_point_lights;
        std::vector<SpotLight*>        m_spot_lights;
        std::vector<CapsuleLight*>     m_capsule_lights;

        eProjectionType m_projection_type = eProjectionType::Perspective;
        Matrix44        m_view_matrix;
        Matrix44        m_projection_matrix;
        Matrix44        m_orthogonal_matrix;

        bool m_b_deferred_shading = false;
    };
}

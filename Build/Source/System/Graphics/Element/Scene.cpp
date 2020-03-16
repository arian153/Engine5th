#include "Scene.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "../Shader/ShaderManager.hpp"
#include "../Shader/ColorShaderCommon.hpp"
#include "../Utility/MatrixManager.hpp"

namespace Engine5
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::Initialize()
    {
        m_camera = new Camera();
        m_camera->SetPosition(Vector3(0.0f, 0.0f, -5.0f));
        m_camera->Initialize();
        //m_mesh = new Mesh();
        //m_mesh->Initialize();
    }

    void Scene::Update(Real dt) const
    {
        m_camera->Update(dt);

        
        //auto world = DirectX::XMMatrixIdentity();
        //auto view  = m_camera->GetViewMatrix();
        //auto proj  = m_renderer->GetPerspectiveMatrix();
        //m_shader_manager->GetColorShader()->Update(dt, m_renderer->GetDeviceContext(), m_mesh->GetIndexCount(), world, view, proj);
    }

    void Scene::Shutdown()
    {
        if (m_camera != nullptr)
        {
            m_camera->Shutdown();
            delete m_camera;
            m_camera = nullptr;
        }
    }

    void Scene::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void Scene::SetShaderManager(ShaderManager* shader_manager)
    {
        m_shader_manager = shader_manager;
    }

    void Scene::SetMatrixManager(MatrixManager* matrix_manager)
    {
        m_matrix_manager = matrix_manager;
    }

    void Scene::DrawShader(eShaderType shader_type)
    {
        switch (shader_type)
        {
        case eShaderType::Color:
            //m_shader_manager->GetColorShader()->Render();
            break;
        default:
            break;
        }
    }

    void Scene::SetProjectionType(eProjectionType projection_type)
    {
        m_projection_type = projection_type;
        UpdateProjection();
    }

    void Scene::UpdateProjection()
    {
        switch (m_projection_type)
        {
        case eProjectionType::Perspective:
            m_projection_matrix = m_matrix_manager->GetPerspectiveMatrix();
            break;
        case eProjectionType::OrthoGraphic:
            m_projection_matrix = m_matrix_manager->GetOrthoGraphicMatrix();
            break;
        default:
            break;
        }
    }
}

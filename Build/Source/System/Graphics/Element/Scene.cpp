#include "Scene.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "../Shader/ShaderManager.hpp"

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
        m_mesh = new Mesh();
        m_mesh->Initialize();
    }

    void Scene::Update(Real dt) const
    {
        m_camera->Update(dt);
        //auto world = DirectX::XMMatrixIdentity();
        //auto view  = m_camera->GetViewMatrix();
        //auto proj  = m_renderer->GetProjectionMatrix();
        m_mesh->Update(dt);
        //m_shader_manager->GetColorShader()->Update(dt, m_renderer->GetDeviceContext(), m_mesh->GetIndexCount(), world, view, proj);
    }

    void Scene::Shutdown()
    {
        if (m_mesh != nullptr)
        {
            m_mesh->Shutdown();
            delete m_mesh;
            m_mesh = nullptr;
        }
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
}

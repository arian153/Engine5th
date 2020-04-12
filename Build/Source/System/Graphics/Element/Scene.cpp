#include "Scene.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "../Shader/ShaderManagerCommon.hpp"
#include "../Utility/MatrixManager.hpp"
#include "../Utility/PrimitiveRenderer.hpp"
#include "../Light/DirectionalLight.hpp"

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
        m_matrix_manager->AddScene(this);
        //primitive renderer
        m_primitive_renderer = new PrimitiveRenderer(m_renderer);
        m_primitive_renderer->Initialize(m_shader_manager->GetColorShader());
        UpdateView();
        UpdateProjection();
    }

    void Scene::Update(Real dt) const
    {
       
    }

    void Scene::Render() const
    {
        m_primitive_renderer->Render();
        MatrixData mvp_data;
        mvp_data.projection = m_projection_matrix;
        DirectionalLight light;

        light.m_ambient = Color(0.15f, 0.15f, 0.15f);
        light.m_direction = Vector3(0.0f, 0.0f, 1.0f);
        light.m_specular_power = 32.0f;

        for (auto& camera : m_cameras)
        {
            mvp_data.view = camera->GetViewMatrix();
            for (auto& mesh : m_meshes)
            {
                mvp_data.model = mesh->GetModelMatrix();
                auto type = mesh->GetShaderType();
                switch (type)
                {
                case eShaderType::Color:
                    mesh->RenderColorBuffer();
                    m_shader_manager->RenderColorShader(mesh->GetIndexCount(), mvp_data);
                    break;
                case eShaderType::Texture:
                    mesh->RenderBuffer();
                    m_shader_manager->RenderTextureShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), mesh->GetColor());
                    break;
                case eShaderType::Light:
                    //mesh->RenderBuffer();
                    //m_shader_manager->RenderLightShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), camera, mesh->GetColor(), light);
                    break;
                default:
                    break;
                }
            }


        }
    }

    void Scene::Shutdown()
    {
        m_matrix_manager->RemoveScene(this);
        if (m_primitive_renderer != nullptr)
        {
            m_primitive_renderer->Shutdown();
            delete m_primitive_renderer;
            m_primitive_renderer = nullptr;
        }
        for (auto& camera : m_cameras)
        {
            camera->Shutdown();
            delete camera;
            camera = nullptr;
        }
        m_cameras.clear();
        for (auto& mesh : m_meshes)
        {
            mesh->Shutdown();
            delete mesh;
            mesh = nullptr;
        }
        m_meshes.clear();
    }

    void Scene::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void Scene::SetShaderManager(ShaderManagerCommon* shader_manager)
    {
        m_shader_manager = shader_manager;
    }

    void Scene::SetMatrixManager(MatrixManager* matrix_manager)
    {
        m_matrix_manager = matrix_manager;
    }

    void Scene::SetMainCamera(Camera* camera)
    {
        if (camera != nullptr)
        {
            m_main_camera = camera;
        }
    }

    void Scene::SetProjectionType(eProjectionType projection_type)
    {
        m_projection_type = projection_type;
        UpdateProjection();
    }

    void Scene::UpdateView()
    {
        if (m_main_camera != nullptr)
        {
            m_view_matrix = m_main_camera->GetViewMatrix();
        }
        if (m_primitive_renderer != nullptr)
        {
            m_primitive_renderer->UpdateViewMatrix(m_view_matrix);
        }
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
        if (m_primitive_renderer != nullptr)
        {
            m_primitive_renderer->UpdateProjectionMatrix(m_projection_matrix);
        }
    }

    PrimitiveRenderer* Scene::GetPrimitiveRenderer() const
    {
        return m_primitive_renderer;
    }

    Camera* Scene::AddCamera(Camera* camera)
    {
        if (m_cameras.empty())
        {
            m_main_camera = camera;
        }
        m_cameras.push_back(camera);
        return camera;
    }

    Mesh* Scene::AddMesh(Mesh* mesh)
    {
        m_meshes.push_back(mesh);
        mesh->SetRenderer(m_renderer);
        return mesh;
    }

    void Scene::RemoveCamera(Camera* camera)
    {
        auto found = std::find(m_cameras.begin(), m_cameras.end(), camera);
        m_cameras.erase(found);
    }

    void Scene::RemoveMesh(Mesh* mesh)
    {
        auto found = std::find(m_meshes.begin(), m_meshes.end(), mesh);
        m_meshes.erase(found);
    }
}

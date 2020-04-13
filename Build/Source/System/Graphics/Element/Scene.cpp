#include "Scene.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "../Shader/ShaderManagerCommon.hpp"
#include "../Utility/MatrixManager.hpp"
#include "../Utility/PrimitiveRenderer.hpp"
#include "../Light/DirectionalLight.hpp"
#include "../Buffer/RenderTextureBufferCommon.hpp"
#include "../Buffer/DeferredBufferCommon.hpp"

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
        //render texture buffer
        m_render_texture_buffer = new RenderTextureBufferCommon();
        m_render_texture_buffer->Initialize(
                                            m_renderer,
                                            m_matrix_manager->GetScreenLeft(),
                                            m_matrix_manager->GetScreenRight(),
                                            m_matrix_manager->GetScreenTop(),
                                            m_matrix_manager->GetScreenBottom());
        //deferred buffer
        m_deferred_buffer = new DeferredBufferCommon();
        m_deferred_buffer->Initialize(
                                      m_renderer,
                                      m_matrix_manager->GetScreenWidth(),
                                      m_matrix_manager->GetScreenHeight());
        UpdateView();
        UpdateProjection();
    }

    void Scene::Update(Real dt)
    {
        if (m_deferred_meshes.empty() == false)
        {
           
            MatrixData mvp_data;
            mvp_data.projection  = m_projection_matrix;
            m_b_deferred_shading = true;
            // Set the render buffers to be the render target.
            m_deferred_buffer->SetRenderTargets();
            // Clear the render buffers.
            m_deferred_buffer->ClearRenderTargets(Color(1.0f, 1.0f, 1.0f, 1.0f));

            

            for (auto& camera : m_cameras)
            {
                mvp_data.view = camera->GetViewMatrix();
                for (auto& mesh : m_deferred_meshes)
                {
                    mvp_data.model = mesh->GetModelMatrix();
                    auto type      = mesh->GetShaderType();
                    mesh->RenderBuffer();
                    switch (type)
                    {
                    case eShaderType::Light:
                        m_shader_manager->RenderDeferredShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), mesh->GetColor());
                        break;
                    default:
                        break;
                    }
                }
            }
            //m_deferred_buffer->ReleaseRenderTarget();
            // Reset the render target back to the original back buffer and not the render buffers anymore.
            m_renderer->SetBackBufferRenderTarget();
            // Reset the viewport back to the original.
            m_renderer->ResetViewport();
        }
    }

    void Scene::Render() const
    {
        m_primitive_renderer->Render();
        MatrixData mvp_data;
       
        DirectionalLight light;
        light.m_ambient        = Color(0.15f, 0.15f, 0.15f);
        light.m_direction      = Vector3(0.0f, 0.0f, 1.0f);
        light.m_specular_power = 32.0f;
        for (auto& camera : m_cameras)
        {
            mvp_data.view = camera->GetViewMatrix();

            //draw per lighting.
            if (m_b_deferred_shading)
            {
                mvp_data.projection = m_orthogonal_matrix;
                mvp_data.model.SetIdentity();
                // Turn off the Z buffer to begin all 2D rendering.
                m_renderer->SetZBuffering(false);
                // Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
                m_render_texture_buffer->Render();
                // Render the full screen ortho window using the deferred light shader and the render buffers.
                m_shader_manager->RenderLightShader(m_render_texture_buffer->GetIndexCount(), mvp_data, m_deferred_buffer, camera, light);
                // Turn the Z buffer back on now that all 2D rendering has completed.
                m_renderer->SetZBuffering(true);
            }

            mvp_data.projection = m_projection_matrix;
            for (auto& mesh : m_forward_meshes)
            {
                mvp_data.model = mesh->GetModelMatrix();
                auto type      = mesh->GetShaderType();
                switch (type)
                {
                case eShaderType::Color:
                    mesh->RenderBuffer();
                    m_shader_manager->RenderColorShader(mesh->GetIndexCount(), mvp_data);
                    break;
                case eShaderType::Texture:
                    mesh->RenderBuffer();
                    m_shader_manager->RenderTextureShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), mesh->GetColor());
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
        if (m_deferred_buffer != nullptr)
        {
            m_deferred_buffer->Shutdown();
            delete m_deferred_buffer;
            m_deferred_buffer = nullptr;
        }
        if (m_render_texture_buffer != nullptr)
        {
            m_render_texture_buffer->Shutdown();
            delete m_render_texture_buffer;
            m_render_texture_buffer = nullptr;
        }
        for (auto& camera : m_cameras)
        {
            camera->Shutdown();
            delete camera;
            camera = nullptr;
        }
        m_cameras.clear();
        for (auto& mesh : m_forward_meshes)
        {
            mesh->Shutdown();
            delete mesh;
            mesh = nullptr;
        }
        m_forward_meshes.clear();
        for (auto& mesh : m_deferred_meshes)
        {
            mesh->Shutdown();
            delete mesh;
            mesh = nullptr;
        }
        m_deferred_meshes.clear();
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
            m_orthogonal_matrix = m_matrix_manager->GetOrthoGraphicMatrix();
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

    void Scene::OnResize() const
    {
        if (m_render_texture_buffer != nullptr)
        {
            m_render_texture_buffer->OnResize(
                                              m_matrix_manager->GetScreenLeft(),
                                              m_matrix_manager->GetScreenRight(),
                                              m_matrix_manager->GetScreenTop(),
                                              m_matrix_manager->GetScreenBottom());
        }
        if (m_deferred_buffer != nullptr)
        {
            m_deferred_buffer->OnResize(
                                        m_matrix_manager->GetScreenWidth(),
                                        m_matrix_manager->GetScreenHeight());
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
        if (mesh->IsDeferred())
        {
            m_deferred_meshes.push_back(mesh);
        }
        else
        {
            m_forward_meshes.push_back(mesh);
        }
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
        if (mesh->IsDeferred() && mesh != nullptr)
        {
            auto found = std::find(m_deferred_meshes.begin(), m_deferred_meshes.end(), mesh);
            m_deferred_meshes.erase(found);
        }
        else
        {
            auto found = std::find(m_forward_meshes.begin(), m_forward_meshes.end(), mesh);
            m_forward_meshes.erase(found);
        }
    }

    void Scene::ChangeShaderType(Mesh* mesh)
    {
        //remove
        auto found_forward = std::find(m_forward_meshes.begin(), m_forward_meshes.end(), mesh);
        if (found_forward != m_forward_meshes.end())
        {
            m_forward_meshes.erase(found_forward);
        }
        else
        {
            auto found_deferred = std::find(m_deferred_meshes.begin(), m_deferred_meshes.end(), mesh);
            if (found_deferred != m_deferred_meshes.end())
            {
                m_deferred_meshes.erase(found_deferred);
            }
        }
        //add
        if (mesh->IsDeferred())
        {
            m_deferred_meshes.push_back(mesh);
        }
        else
        {
            m_forward_meshes.push_back(mesh);
        }
    }
}

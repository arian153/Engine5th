#include "Scene.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "../Common/Shader/ShaderManagerCommon.hpp"
#include "../Utility/MatrixManager.hpp"
#include "../Utility/PrimitiveRenderer.hpp"
#include "../Common/Light/DirectionalLight.hpp"
#include "../Common/Buffer/DeferredBufferCommon.hpp"
#include "../Common/Light/PointLight.hpp"
#include "../Common/Light/SpotLight.hpp"
#include "../Common/Light/CapsuleLight.hpp"
#include "ParticleEmitter.hpp"
#include "../../../Manager/Resource/ResourceManager.hpp"
#include "Mesh2.hpp"
#include "../Utility/TextRenderer.hpp"
#include "../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../Common/Buffer2/ConstantBufferData.hpp"
#include "../Common/Buffer2/VertexLayoutCommon.hpp"
#include "../Common/Element/Mesh.hpp"
#include "../Common/Element/TextSprite.hpp"
#include "../Common/Light/LightDef.hpp"
#include "../Common/Shader/ShaderProgramCommon.hpp"
#include "../DataType/MatrixData.hpp"

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
        //deferred buffer
        m_deferred_buffer = new DeferredBufferCommon();
        m_deferred_buffer->Initialize(
                                      m_renderer,
                                      m_matrix_manager->GetScreenWidth(),
                                      m_matrix_manager->GetScreenHeight());
        UpdateView();
        UpdateProjection();

        m_matrix_buffer = new ConstantBufferCommon();
        m_matrix_buffer->Init(m_renderer, eBindingStage::VertexShader, sizeof(MatrixBufferData), 0);

        m_color_vertex_layout = new VertexLayoutCommon();
        m_color_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
        m_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 0, eInputSlotType::VERTEX_DATA, 0, 0);

        m_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
        m_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
        m_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
        m_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 3, eInputSlotType::INSTANCE_DATA, 1, 1);
        m_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 1, eInputSlotType::INSTANCE_DATA, 1, 1);

        m_new_color_shader = new ShaderProgramCommon(m_shader_manager);
        m_new_color_shader->SetShaderResource(m_resource_manager->GetShaderResourceFileName(L"Color.hlsl"));
        m_new_color_shader->SetVertexLayout(m_color_vertex_layout);
        m_new_color_shader->AddConstantBuffer(m_matrix_buffer);
        m_new_color_shader->Init(m_renderer);

        m_test_mesh = new Mesh2();
        m_test_mesh->Initialize();
        m_test_mesh->SetRenderer(m_renderer);
        m_test_mesh->BuildBuffer();
    }

    void Scene::Update(Real dt)
    {
        if (m_main_camera == nullptr || m_cameras.empty())
            return;
        MatrixData mvp_data;
        mvp_data.projection = m_projection_matrix;
        m_main_camera->Update();
        mvp_data.view = m_main_camera->GetViewMatrix();
        m_frustum.ConstructFrustum(m_matrix_manager->GetFarPlane(), mvp_data.projection, mvp_data.view);
        m_primitive_renderer->UpdateFrustum(m_frustum);
        m_b_deferred_shading = false;
        if (m_deferred_meshes.empty() == false)
        {
            m_b_deferred_shading = true;
            // Set the render buffers to be the render target.
            m_deferred_buffer->SetRenderTargets();
            // Clear the render buffers.
            m_deferred_buffer->ClearRenderTargets(Color(0.0f, 0.0f, 0.0f, 0.0f));
            for (auto& camera : m_cameras)
            {
                camera->Update();
                mvp_data.view = camera->GetViewMatrix();
                for (auto& mesh : m_deferred_meshes)
                {
                    mvp_data.model = mesh->GetModelMatrix();
                    auto type      = mesh->GetShaderType();
                    switch (type)
                    {
                    case eShaderType::DeferredDirectionalLight:
                        mesh->RenderBuffer();
                        m_shader_manager->RenderDeferredBufferShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), mesh->GetColor());
                        break;
                    default:
                        break;
                    }
                }
            }
            // Reset the render target back to the original back buffer and not the render buffers anymore.
            m_renderer->SetBackBufferRenderTarget();
            // Reset the viewport back to the original.
            m_renderer->ResetViewport();
        }
        for (auto& particle : m_particle_emitters)
        {
            particle->Update(dt);
        }
    }

    void Scene::Render() const
    {
        if (m_main_camera == nullptr || m_cameras.empty())
            return;
        m_primitive_renderer->Render();
        m_primitive_renderer->Clear();
        MatrixData mvp_data;
        if (m_b_deferred_shading)
        {
            mvp_data.projection = m_orthogonal_matrix;
            mvp_data.model.SetIdentity();
            for (auto& camera : m_cameras)
            {
                mvp_data.view = camera->GetViewMatrix();
                m_renderer->SetZBuffering(false);
                for (DirectionalLight* directional_light : m_directional_lights)
                {
                    m_deferred_buffer->Render();
                    m_shader_manager->RenderDeferredDirectionalLightShader(
                                                                           m_deferred_buffer->GetIndexCount(),
                                                                           mvp_data,
                                                                           m_deferred_buffer,
                                                                           camera,
                                                                           directional_light
                                                                          );
                }
                /*for (PointLight* point_light : m_point_lights)
                {
                    m_render_texture_buffer->Render();
                    m_shader_manager->RenderDeferredLightShader(
                                                                m_render_texture_buffer->GetIndexCount(),
                                                                mvp_data,
                                                                m_deferred_buffer,
                                                                camera,
                                                                point_light
                                                               );
                }
                for (SpotLight* spot_light : m_spot_lights)
                {
                    m_render_texture_buffer->Render();
                    m_shader_manager->RenderDeferredLightShader(
                                                                m_render_texture_buffer->GetIndexCount(),
                                                                mvp_data,
                                                                m_deferred_buffer,
                                                                camera,
                                                                spot_light);
                }
                for (CapsuleLight* capsule_light : m_capsule_lights)
                {
                    m_render_texture_buffer->Render();
                    m_shader_manager->RenderDeferredLightShader(
                                                                m_render_texture_buffer->GetIndexCount(),
                                                                mvp_data,
                                                                m_deferred_buffer,
                                                                camera,
                                                                capsule_light);
                }*/
                m_renderer->SetZBuffering(true);
            }
        }
        mvp_data.projection = m_projection_matrix;
        for (auto& camera : m_cameras)
        {
            camera->Update();
            mvp_data.view = camera->GetViewMatrix();

            MatrixBufferData data;
            data.proj = m_projection_matrix;
            data.view = camera->GetViewMatrix();

            /* MatrixBufferData* mapped_data = (MatrixBufferData*)m_matrix_buffer->Map();
             mapped_data->model = Matrix44();
             mapped_data->view = camera->GetViewMatrix().Transpose();
             mapped_data->proj = m_projection_matrix.Transpose();
             m_matrix_buffer->UnMap();*/
            m_matrix_buffer->Update(data);

            m_new_color_shader->Bind();
            m_test_mesh->Render();

            for (auto& mesh : m_other_meshes)
            {
                mvp_data.model = mesh->GetModelMatrix();
                auto type      = mesh->GetShaderType();
                if (type == eShaderType::Color)
                {
                    mesh->RenderBuffer();
                    m_shader_manager->RenderColorShader(mesh->GetIndexCount(), mvp_data);
                }
                else if (type == eShaderType::Texture)
                {
                    mesh->RenderBuffer();
                    m_shader_manager->RenderTextureShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), mesh->GetColor());
                }
                else if (type == eShaderType::MultiTexture)
                {
                    mesh->RenderBuffer();
                    m_shader_manager->RenderMultiTextureShader(mesh->GetIndexCount(), mvp_data, mesh->GetTextureArray(), mesh->GetColor(), 1.7f);
                }
                else if (type == eShaderType::AlphaMapping)
                {
                    mesh->RenderBuffer();
                    m_shader_manager->RenderAlphaMappingShader(mesh->GetIndexCount(), mvp_data, mesh->GetTextureArray(), mesh->GetColor());
                }
                else if (type == eShaderType::LightMapping)
                {
                    mesh->RenderBuffer();
                    m_shader_manager->RenderLightMappingShader(mesh->GetIndexCount(), mvp_data, mesh->GetTextureArray(), mesh->GetColor());
                }
            }
            for (auto& mesh : m_forward_meshes)
            {
                mvp_data.model = mesh->GetModelMatrix();
                auto type      = mesh->GetShaderType();
                for (DirectionalLight* directional_light : m_directional_lights)
                {
                    if (type == eShaderType::ForwardDirectionalLight)
                    {
                        mesh->RenderBuffer();
                        m_shader_manager->RenderForwardDirectionalLightShader(
                                                                              mesh->GetIndexCount(),
                                                                              mvp_data,
                                                                              mesh->GetTexture(),
                                                                              camera,
                                                                              mesh->GetColor(),
                                                                              directional_light
                                                                             );
                    }
                    else if (type == eShaderType::NormalMapping)
                    {
                        mesh->RenderBuffer();
                        m_shader_manager->RenderNormalMappingShader(
                                                                    mesh->GetIndexCount(),
                                                                    mvp_data,
                                                                    mesh->GetTextureArray(),
                                                                    mesh->GetColor(),
                                                                    directional_light
                                                                   );
                    }
                    else if (type == eShaderType::SpecularMapping)
                    {
                        mesh->RenderBuffer();
                        m_shader_manager->RenderSpecularMappingShader(
                                                                      mesh->GetIndexCount(),
                                                                      mvp_data,
                                                                      mesh->GetTextureArray(),
                                                                      camera,
                                                                      mesh->GetColor(),
                                                                      directional_light
                                                                     );
                    }
                }
            }
            for (auto& text_sprite : m_text_sprites)
            {
                mvp_data.model = text_sprite->GetModelMatrix();
                text_sprite->Render();
                m_shader_manager->RenderTextureShader(
                                                      text_sprite->GetIndexCount(),
                                                      mvp_data,
                                                      text_sprite->GetTexture(),
                                                      text_sprite->GetColor());
            }
            for (auto& particle : m_particle_emitters)
            {
                mvp_data.model = particle->GetModelMatrix();
                particle->SetBillboardPosition(camera->GetPosition());
                m_shader_manager->RenderInstanceTextureShader(
                                                              mvp_data,
                                                              particle->GetTexture(),
                                                              ColorDef::Pure::White);
                particle->Render();
            }
        }
    }

    void Scene::Shutdown()
    {
        {
            m_matrix_buffer->Shutdown();
            delete m_matrix_buffer;
            m_matrix_buffer = nullptr;

            m_color_vertex_layout->Clear();
            delete m_color_vertex_layout;
            m_color_vertex_layout = nullptr;

            m_new_color_shader->Shutdown();
            delete m_new_color_shader;
            m_new_color_shader = nullptr;

            m_test_mesh->Shutdown();
            delete m_test_mesh;
            m_test_mesh = nullptr;
        }

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
        for (auto& mesh : m_other_meshes)
        {
            mesh->Shutdown();
            delete mesh;
            mesh = nullptr;
        }
        m_other_meshes.clear();
        for (auto& light : m_directional_lights)
        {
            light->Shutdown();
            delete light;
            light = nullptr;
        }
        m_directional_lights.clear();
        for (auto& light : m_point_lights)
        {
            light->Shutdown();
            delete light;
            light = nullptr;
        }
        m_point_lights.clear();
        for (auto& light : m_spot_lights)
        {
            light->Shutdown();
            delete light;
            light = nullptr;
        }
        m_spot_lights.clear();
        for (auto& light : m_capsule_lights)
        {
            light->Shutdown();
            delete light;
            light = nullptr;
        }
        m_capsule_lights.clear();
        for (auto& text_sprite : m_text_sprites)
        {
            text_sprite->Shutdown();
            delete text_sprite;
            text_sprite = nullptr;
        }
        m_text_sprites.clear();
        for (auto& particle : m_particle_emitters)
        {
            particle->Shutdown();
            delete particle;
            particle = nullptr;
        }
        m_particle_emitters.clear();
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

    void Scene::SetResourceManager(ResourceManager* resource_manager)
    {
        m_resource_manager = resource_manager;
    }

    void Scene::SetTextRenderer(TextRenderer* text_renderer)
    {
        m_text_renderer = text_renderer;
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
        auto lighting = mesh->GetLightingMethod();
        if (lighting == eLightingMethod::Deferred)
        {
            m_deferred_meshes.push_back(mesh);
        }
        else if (lighting == eLightingMethod::Forward)
        {
            m_forward_meshes.push_back(mesh);
        }
        else
        {
            m_other_meshes.push_back(mesh);
        }
        mesh->SetRenderer(m_renderer);
        return mesh;
    }

    void Scene::AddLight(DirectionalLight* light)
    {
        m_directional_lights.push_back(light);
    }

    void Scene::AddLight(PointLight* light)
    {
        m_point_lights.push_back(light);
    }

    void Scene::AddLight(SpotLight* light)
    {
        m_spot_lights.push_back(light);
    }

    void Scene::AddLight(CapsuleLight* light)
    {
        m_capsule_lights.push_back(light);
    }

    void Scene::AddTextSprite(TextSprite* text_sprite)
    {
        m_text_sprites.push_back(text_sprite);
    }

    void Scene::AddParticleEmitter(ParticleEmitter* particle_emitter)
    {
        m_particle_emitters.push_back(particle_emitter);
    }

    void Scene::RemoveCamera(Camera* camera)
    {
        auto found = std::find(m_cameras.begin(), m_cameras.end(), camera);
        m_cameras.erase(found);
    }

    void Scene::RemoveMesh(Mesh* mesh)
    {
        if (mesh != nullptr)
        {
            auto lighting = mesh->GetLightingMethod();
            if (lighting == eLightingMethod::Deferred)
            {
                if (m_deferred_meshes.empty() == false)
                {
                    auto found = std::find(m_deferred_meshes.begin(), m_deferred_meshes.end(), mesh);
                    m_deferred_meshes.erase(found);
                }
            }
            else if (lighting == eLightingMethod::Forward)
            {
                if (m_forward_meshes.empty() == false)
                {
                    auto found = std::find(m_forward_meshes.begin(), m_forward_meshes.end(), mesh);
                    m_forward_meshes.erase(found);
                }
            }
            else
            {
                if (m_other_meshes.empty() == false)
                {
                    auto found = std::find(m_other_meshes.begin(), m_other_meshes.end(), mesh);
                    m_other_meshes.erase(found);
                }
            }
        }
    }

    void Scene::RemoveLight(DirectionalLight* light)
    {
        auto found = std::find(m_directional_lights.begin(), m_directional_lights.end(), light);
        m_directional_lights.erase(found);
    }

    void Scene::RemoveLight(PointLight* light)
    {
        auto found = std::find(m_point_lights.begin(), m_point_lights.end(), light);
        m_point_lights.erase(found);
    }

    void Scene::RemoveLight(SpotLight* light)
    {
        auto found = std::find(m_spot_lights.begin(), m_spot_lights.end(), light);
        m_spot_lights.erase(found);
    }

    void Scene::RemoveLight(CapsuleLight* light)
    {
        auto found = std::find(m_capsule_lights.begin(), m_capsule_lights.end(), light);
        m_capsule_lights.erase(found);
    }

    void Scene::RemoveTextSprite(TextSprite* text_sprite)
    {
        auto found = std::find(m_text_sprites.begin(), m_text_sprites.end(), text_sprite);
        m_text_sprites.erase(found);
    }

    void Scene::RemoveParticleEmitter(ParticleEmitter* particle_emitter)
    {
        auto found = std::find(m_particle_emitters.begin(), m_particle_emitters.end(), particle_emitter);
        m_particle_emitters.erase(found);
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
            else
            {
                auto found_others = std::find(m_other_meshes.begin(), m_other_meshes.end(), mesh);
                if (found_others != m_other_meshes.end())
                {
                    m_other_meshes.erase(found_others);
                }
            }
        }
        //add
        auto lighting = mesh->GetLightingMethod();
        if (lighting == eLightingMethod::Deferred)
        {
            m_deferred_meshes.push_back(mesh);
        }
        else if (lighting == eLightingMethod::Forward)
        {
            m_forward_meshes.push_back(mesh);
        }
        else
        {
            m_other_meshes.push_back(mesh);
        }
    }

    void Scene::InitializeTextSprite(TextSprite* text_sprite) const
    {
        text_sprite->Initialize(m_renderer);
    }

    void Scene::InitializeParticleEmitter(ParticleEmitter* particle_emitter) const
    {
        particle_emitter->SetRenderer(m_renderer);
        particle_emitter->SetBuffer();
    }

    Basis Scene::GetMainCameraBasis() const
    {
        return m_main_camera->GetBasis();
    }

    Real Scene::GetAspectRatio() const
    {
        return m_matrix_manager->GetAspectRatio();
    }

    Ray Scene::GetPickingRay(const Vector2& pos) const
    {
        Matrix33 proj(m_projection_matrix);
        Vector2  point;
        point.x = pos.x / proj(0, 0);
        point.y = pos.y / proj(1, 1);

        Matrix33 inv_view(m_main_camera->GetViewMatrix().Inverse());
        Vector3  origin(m_main_camera->GetPosition());
        Vector3  dir;
        dir.x = (point.x * inv_view(0, 0)) + (point.y * inv_view(1, 0)) + inv_view(2, 0);
        dir.y = (point.x * inv_view(0, 1)) + (point.y * inv_view(1, 1)) + inv_view(2, 1);
        dir.z = (point.x * inv_view(0, 2)) + (point.y * inv_view(1, 2)) + inv_view(2, 2);

        //m_main_camera->GetTransform()->LocalToWorldMatrix();

        dir.SetNormalize();

        Ray ray(origin, dir);
        return ray;
    }
}

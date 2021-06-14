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
#include "TextSprite.hpp"
#include "../../../Manager/Resource/ResourceType/MeshResource.hpp"
#include "../../../Manager/Resource/ResourceType/TextureResource.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../Common/Light/LightDef.hpp"
#include "../Common/Shader/ShaderProgramCommon.hpp"
#include "../DataType/MatrixData.hpp"
#include "../Utility/MaterialManager.hpp"

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
        m_primitive_renderer->Initialize(m_shader_manager->GetShader("Color"));
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

        m_matrix_instancing_buffer = new ConstantBufferCommon();
        m_matrix_instancing_buffer->Init(m_renderer, eBindingStage::VertexShader, sizeof(MatrixBufferDataInstancing), 0);

        m_shader_manager->AddBuffer("Color", m_matrix_buffer);
        m_shader_manager->AddBuffer("ColorInstancing", m_matrix_instancing_buffer);

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

        MatrixBufferData mvp_buffer;
        mvp_buffer.view = m_main_camera->GetViewMatrix();
        mvp_buffer.proj = m_projection_matrix;
        m_matrix_buffer->Update(mvp_buffer);

        MatrixBufferDataInstancing vp_data;
        vp_data.view = m_main_camera->GetViewMatrix();
        vp_data.proj = m_projection_matrix;
        m_matrix_instancing_buffer->Update(vp_data);

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
                        //m_shader_manager->RenderDeferredBufferShader(mesh->GetIndexCount(), mvp_data, mesh->GetTexture(), mesh->GetColor());
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
                m_renderer->SetZBuffering(true);
            }
        }
        mvp_data.projection = m_projection_matrix;

        m_shader_manager->Bind("ColorInstancing");
        m_test_mesh->Render();

        for (auto& camera : m_cameras)
        {
            camera->Update();
            mvp_data.view = camera->GetViewMatrix();

            for (auto& mesh : m_other_meshes)
            {
                mvp_data.model = mesh->GetModelMatrix();
                auto type      = mesh->GetShaderType();
                /*   if (type == eShaderType::Color)
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
                   }*/
            }
            for (auto& mesh : m_forward_meshes)
            {
                mvp_data.model = mesh->GetModelMatrix();
                auto type      = mesh->GetShaderType();
                for (DirectionalLight* directional_light : m_directional_lights)
                {
                    /* if (type == eShaderType::ForwardDirectionalLight)
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
                     }*/
                }
            }
            /*  for (auto& text_sprite : m_text_sprites)
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
              }*/
        }
    }

    void Scene::Shutdown()
    {
        {
            m_matrix_buffer->Shutdown();
            delete m_matrix_buffer;
            m_matrix_buffer = nullptr;

            m_matrix_instancing_buffer->Shutdown();
            delete m_matrix_instancing_buffer;
            m_matrix_instancing_buffer = nullptr;

            m_test_mesh->Shutdown();
            delete m_test_mesh;
            m_test_mesh = nullptr;
        }

        for (auto& [fst1, mesh_table] : m_meshes)
        {
            for (auto& [fst2, mesh] : mesh_table)
            {
                if (mesh != nullptr)
                {
                    mesh->Shutdown();
                    delete mesh;
                    mesh = nullptr;
                }
            }
            mesh_table.clear();
        }
        m_meshes.clear();

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

    void Scene::SetMaterialManager(MaterialManager* material_manager)
    {
        m_material_manager = material_manager;
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

    Mesh2* Scene::GetMesh(size_t model_id, size_t material_id)
    {
        auto found_model = m_meshes.find(model_id);
        if (found_model != m_meshes.end())
        {
            auto& material_table = found_model->second;
            auto  found_material = material_table.find(material_id);

            if (found_material != material_table.end())
            {
                return found_material->second;
            }
        }
        return nullptr;
    }

    Mesh2* Scene::GetMesh(const std::string& model_path, const MaterialData& material)
    {
        Mesh2*        created;
        MeshResource* model_resource = m_resource_manager->GetMeshResource(ToWString(model_path));
        size_t        model_id       = reinterpret_cast<size_t>(model_resource);
        size_t        material_id    = m_material_manager->GetID(material);

        auto found_model = m_meshes.find(model_id);

        if (found_model == m_meshes.end())
        {
            //add new
            created = new Mesh2();
            SetUpMesh(created, model_resource->GetMeshData(), material, model_id, material_id);
            m_meshes.emplace(model_id, std::unordered_map<size_t, Mesh2*>({{material_id, created}}));
        }
        else
        {
            auto& material_table = found_model->second;
            auto  found_material = material_table.find(material_id);

            if (found_material == material_table.end())
            {
                //add new
                created = new Mesh2();
                SetUpMesh(created, model_resource->GetMeshData(), material, model_id, material_id);
                material_table.emplace(material_id, created);
            }
            else
            {
                created = found_material->second;
            }
        }

        return created;
    }

    Mesh2* Scene::GetMesh(MeshData* model_data, const MaterialData& material)
    {
        Mesh2* created;
        size_t model_id    = reinterpret_cast<size_t>(model_data);
        size_t material_id = m_material_manager->GetID(material);
        auto   found_model = m_meshes.find(model_id);

        if (found_model == m_meshes.end())
        {
            //add new
            created = new Mesh2();
            SetUpMesh(created, model_data, material, model_id, material_id);
            m_meshes.emplace(model_id, std::unordered_map<size_t, Mesh2*>({{material_id, created}}));
        }
        else
        {
            auto& material_table = found_model->second;
            auto  found_material = material_table.find(material_id);

            if (found_material == material_table.end())
            {
                //add new
                created = new Mesh2();
                SetUpMesh(created, model_data, material, model_id, material_id);
                material_table.emplace(material_id, created);
            }
            else
            {
                created = found_material->second;
            }
        }

        return created;
    }

    void Scene::SetUpMesh(Mesh2* mesh, MeshData* model_data, const MaterialData& material, size_t model_id, size_t material_id) const
    {
        if (mesh != nullptr)
        {
            mesh->SetModelData(model_data);
            mesh->SetSceneID(model_id, material_id);
            mesh->SetMaterialData(material);
            //diffuse texture0
            if (material.diffuse0 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.diffuse0))->GetTexture());
            }
            else
            {
                mesh->AddTexture(nullptr);
            }
            //diffuse texture1
            if (material.diffuse1 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.diffuse1))->GetTexture());
            }
            else
            {
                mesh->AddTexture(nullptr);
            }
            //diffuse texture2
            if (material.diffuse2 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.diffuse2))->GetTexture());
            }
            else
            {
                mesh->AddTexture(nullptr);
            }
            //specular texture
            if (material.specular0 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.specular0))->GetTexture());
            }
            else
            {
                mesh->AddTexture(nullptr);
            }
            //normal texture
            if (material.normal0 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.normal0))->GetTexture());
            }
            else
            {
                mesh->AddTexture(nullptr);
            }
        }
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

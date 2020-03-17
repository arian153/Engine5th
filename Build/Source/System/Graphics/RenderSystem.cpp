#include "RenderSystem.hpp"
#include "Renderer/RendererCommon.hpp"
#include "Shader/ShaderManager.hpp"
#include "../Core/OS-API/OSCommon.hpp"

namespace Engine5
{
    RenderSystem::RenderSystem(OSCommon* operating_system)
        : m_operating_system(operating_system)
    {
    }

    RenderSystem::~RenderSystem()
    {
    }

    void RenderSystem::Initialize(int rendering_width, int rendering_height, ResourceManager* resource_manager)
    {
        //renderer
        m_renderer = new RendererCommon();
        m_renderer->SetHwnd(m_operating_system->AppHWnd());
        m_renderer->Initialize(rendering_width, rendering_height, m_operating_system->IsFullscreen());
        //shader
        m_resource_manager = resource_manager;
        m_shader_manager   = new ShaderManager();
        m_shader_manager->Initialize(m_renderer, resource_manager);
        //matrix generator
        m_matrix_manager = new MatrixManager();
        m_matrix_manager->SetClientRect(rendering_width, rendering_height);
        //primitive renderer
        m_primitive_renderer = new PrimitiveRenderer(m_renderer);
        m_primitive_renderer->Initialize(m_shader_manager->GetColorShader(), m_matrix_manager);
        m_primitive_renderer->SetRendererCameraPosition(Vector3(0.0f, 0.0f, -5.0f));
        m_primitive_renderer->UpdateProjectionMatrix();

        if(m_operating_system->WindowMode() == eWindowMode::Fullscreen)
        {
            m_operating_system->SetWindowMode(eWindowMode::Fullscreen);
            m_renderer->OnFullscreen(true);
        }
        
    }

    void RenderSystem::Shutdown()
    {
        for (auto& scene : m_scenes)
        {
            scene->Shutdown();
            delete scene;
            scene = nullptr;
        }
        m_scenes.clear();
        if (m_primitive_renderer != nullptr)
        {
            m_primitive_renderer->Shutdown();
            delete m_primitive_renderer;
            m_primitive_renderer = nullptr;
        }
        if (m_matrix_manager != nullptr)
        {
            delete m_matrix_manager;
            m_matrix_manager = nullptr;
        }
        if (m_renderer != nullptr)
        {
            m_renderer->Shutdown();
            delete m_renderer;
            m_renderer = nullptr;
        }
        if (m_shader_manager != nullptr)
        {
            m_shader_manager->Shutdown();
            delete m_shader_manager;
            m_shader_manager = nullptr;
        }
    }

    void RenderSystem::BeginUpdate() const
    {
        m_renderer->BeginScene(m_background_color);
        m_primitive_renderer->Update();
    }

    void RenderSystem::EndUpdate() const
    {
        m_renderer->EndScene();
    }

    void RenderSystem::OnResize(int width, int height) const
    {
        if (m_renderer != nullptr && m_renderer->IsInit())
        {
            m_renderer->OnResize(width, height, m_operating_system->IsFullscreen());
            m_matrix_manager->SetClientRect(static_cast<size_t>(width), static_cast<size_t>(height));
            m_primitive_renderer->UpdateProjectionMatrix();
        }
    }

    void RenderSystem::SetVSyncEnable(bool vsync_flag)
    {
        m_b_vsync = vsync_flag;
        m_renderer->SetVSync(vsync_flag);
    }

    bool RenderSystem::IsVSyncEnable() const
    {
        return m_b_vsync;
    }

    void RenderSystem::SetFarNearPlane(Real far_plane, Real near_plane) const
    {
        m_matrix_manager->SetFarPlane(far_plane);
        m_matrix_manager->SetNearPlane(near_plane);
        m_primitive_renderer->UpdateProjectionMatrix();
    }

    void RenderSystem::SetFieldOfView(Real field_of_view) const
    {
        m_matrix_manager->SetFieldOfView(field_of_view);
        m_primitive_renderer->UpdateProjectionMatrix();
    }

    void RenderSystem::SetBackgroundColor(const Color& color)
    {
        m_background_color = color;
    }

    Color RenderSystem::GetBackgroundColor() const
    {
        return m_background_color;
    }

    Scene* RenderSystem::CreateScene()
    {
        Scene* scene = new Scene();
        m_scenes.push_back(scene);
        scene->SetShaderManager(m_shader_manager);
        scene->SetRenderer(m_renderer);
        scene->SetMatrixManager(m_matrix_manager);
        scene->Initialize();
        scene->UpdateProjection();
        return scene;
    }

    void RenderSystem::RemoveScene(Scene* scene)
    {
        if (scene != nullptr)
        {
            m_scenes.erase(std::find(m_scenes.begin(), m_scenes.end(), scene));
            scene->Shutdown();
            delete scene;
            scene = nullptr;
        }
    }
}

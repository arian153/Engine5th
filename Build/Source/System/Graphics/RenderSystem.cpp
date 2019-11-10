#include "RenderSystem.hpp"
#include "../Core/OS-API/Windows/Windows.hpp"
#include "Utility/Color.hpp"

namespace Engine5
{
    RenderSystem::RenderSystem(WindowsAPI* os_api)
        : m_os_api(os_api)
    {
    }

    RenderSystem::~RenderSystem()
    {
    }

    void RenderSystem::Initialize()
    {
        m_renderer = new DirectX3D11(m_os_api->AppHWnd());
        m_renderer->Initialize((int)m_render_width, (int)m_render_height, m_os_api->IsFullscreen(), m_far_plane, m_near_plane, Math::PI_DIV_4);
        m_shader_manager = new ShaderManager(m_renderer->GetDevice(), m_os_api->AppHWnd());
        m_shader_manager->Initialize();
        m_primitive_renderer = new PrimitiveRenderer(m_renderer);
        m_primitive_renderer->Initialize(m_shader_manager->GetColorShader());
        m_primitive_renderer->SetRendererCameraPosition(Vector3(0.0f, 0.0f, -5.0f));
        m_primitive_renderer->UpdateProjectionMatrix();
    }

    void RenderSystem::Update(Real dt)
    {
        if (m_renderer != nullptr)
        {
            m_renderer->BeginScene(m_background_color);
            m_renderer->Update(dt);

            m_primitive_renderer->Update(dt);
            //update scene
            for (auto& scene : m_scenes)
            {
                scene->Update(dt);
            }
            m_renderer->EndScene();
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

    void RenderSystem::OnResize(int width, int height)
    {
        if (m_renderer != nullptr)
        {
            m_render_width  = width;
            m_render_height = height;
            m_renderer->OnResize(width, height, m_os_api->IsFullscreen(), m_far_plane, m_near_plane, m_field_of_view);
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

    void RenderSystem::SetFarNearPlane(Real far_plane, Real near_plane)
    {
        m_far_plane  = far_plane;
        m_near_plane = near_plane;
    }

    void RenderSystem::SetFieldOfView(Real field_of_view)
    {
        m_field_of_view = field_of_view;
    }

    void RenderSystem::SetBackgroundColor(const Color& color)
    {
        m_background_color = color;
    }

    Color RenderSystem::GetBackgroundColor() const
    {
        return m_background_color;
    }
}

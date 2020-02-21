#pragma once
#include "Element/Scene.hpp"
#include <vector>
#include "../Math/Utility/MathDef.hpp"
#include "RenderingAPI/DirectX3D11.hpp"
#include "Utility/Color.hpp"
#include "Utility/ShaderManager.hpp"
#include "Utility/PrimitiveRenderer.hpp"
#include "../Core/OS-API/Windows/Windows.hpp"
#include "Utility/MatrixGenerator.hpp"


namespace Engine5
{
    class OSAPI;

    class RenderSystem
    {
    public:
        explicit RenderSystem(WindowsAPI* os_api);
        ~RenderSystem();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void OnResize(int width, int height);

        void  SetVSyncEnable(bool vsync_flag);
        bool  IsVSyncEnable() const;
        void  SetFarNearPlane(Real far_plane, Real near_plane);
        void  SetFieldOfView(Real field_of_view);
        void  SetBackgroundColor(const Color& color);
        Color GetBackgroundColor() const;

        Scene* CreateScene();
        void RemoveScene(Scene* scene);


    private:
        WindowsAPI*        m_os_api             = nullptr;
        DirectX3D11*       m_renderer           = nullptr;
        ShaderManager*     m_shader_manager     = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;
        MatrixGenerator    m_matrix_generator;

        std::vector<Scene*> m_scenes;

        size_t m_render_width     = 1280;
        size_t m_render_height    = 720;
        bool   m_b_vsync          = true;
        Real   m_far_plane        = 1000.0f;
        Real   m_near_plane       = 0.1f;
        Real   m_field_of_view    = Math::PI_DIV_4;
        Color  m_background_color = ColorDef::Pure::Gray;
    };
}

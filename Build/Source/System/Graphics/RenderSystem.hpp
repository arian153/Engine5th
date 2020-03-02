#pragma once
#include "Element/Scene.hpp"
#include <vector>
#include "../Math/Utility/MathDef.hpp"
#include "Utility/PrimitiveRenderer.hpp"
#include "../Core/OS-API/Windows/Windows.hpp"
#include "Utility/MatrixGenerator.hpp"
#include "DataType/Color.hpp"

namespace Engine5
{
    class OSAPI;
    class RendererCommon;

    class RenderSystem
    {
    public:
        explicit RenderSystem(WindowsAPI* os_api);
        ~RenderSystem();

        void Initialize(int rendering_width, int rendering_height);
        void Update(Real dt);
        void Shutdown();

        void OnResize(int width, int height) const;

        void  SetVSyncEnable(bool vsync_flag);
        bool  IsVSyncEnable() const;
        void  SetFarNearPlane(Real far_plane, Real near_plane) const;
        void  SetFieldOfView(Real field_of_view) const;
        void  SetBackgroundColor(const Color& color);
        Color GetBackgroundColor() const;

        Scene* CreateScene();
        void   RemoveScene(Scene* scene);

    private:
        WindowsAPI*        m_os_api             = nullptr;
        RendererCommon*    m_renderer           = nullptr;
        ShaderManager*     m_shader_manager     = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;
        MatrixGenerator*   m_matrix_generator   = nullptr;

        std::vector<Scene*> m_scenes;
        bool                m_b_vsync          = true;
        Color               m_background_color = ColorDef::Pure::Gray;
    };
}

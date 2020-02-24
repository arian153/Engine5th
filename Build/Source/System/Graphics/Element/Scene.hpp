#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "../Utility/ShaderManager.hpp"

namespace Engine5
{
    class RendererCommon;
    class RendererDX11;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void Initialize();
        void Update(Real dt) const;
        void Shutdown();

        void SetRenderer(RendererCommon* renderer);
        void SetShaderManager(ShaderManager* shader_manager);

    private:
        RendererCommon* m_renderer       = nullptr;
        ShaderManager*  m_shader_manager = nullptr;
        Camera*         m_camera         = nullptr;
        Mesh*           m_mesh           = nullptr;
    };
}

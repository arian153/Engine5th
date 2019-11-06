#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "../RenderingAPI/DirectX3D11.hpp"
#include "../Utility/ShaderManager.hpp"

namespace Engine5
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        void Initialize();
        void Update(Real dt) const;
        void Shutdown();

        void SetDX11(DirectX3D11* m_dx11_api);
        void SetShaderManager(ShaderManager* shader_manager);

    private:
        DirectX3D11*   m_dx11_api       = nullptr;
        ShaderManager* m_shader_manager = nullptr;
        Camera*        m_camera         = nullptr;
        Mesh*          m_mesh           = nullptr;
    };
}

#pragma once
#include "../Shader/ColorShader.hpp"

namespace Engine5
{
    class ShaderManager
    {
    public:
        ShaderManager(ID3D11Device* device, HWND hwnd);
        ~ShaderManager();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        ColorShader* GetColorShader() const;

    private:
        ID3D11Device* m_device = nullptr;
        HWND          m_hwnd   = nullptr;

        //shader list
        ColorShader* m_color_shader = nullptr;
    };
}

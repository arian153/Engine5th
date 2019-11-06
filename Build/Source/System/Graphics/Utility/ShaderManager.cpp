#include "ShaderManager.hpp"

namespace Engine5
{
    ShaderManager::ShaderManager(ID3D11Device* device, HWND hwnd)
        : m_device(device), m_hwnd(hwnd)
    {
    }

    ShaderManager::~ShaderManager()
    {
    }

    void ShaderManager::Initialize()
    {
        m_color_shader = new ColorShader("../../Resource/Shader/Color.vs", "../../Resource/Shader/Color.ps");
        m_color_shader->Initialize(m_device, m_hwnd);
    }

    void ShaderManager::Update(Real dt)
    {
    }

    void ShaderManager::Shutdown()
    {
        if (m_color_shader != nullptr)
        {
            m_color_shader->Shutdown();
            delete m_color_shader;
            m_color_shader = nullptr;
        }
    }

    ColorShader* ShaderManager::GetColorShader() const
    {
        return m_color_shader;
    }
}

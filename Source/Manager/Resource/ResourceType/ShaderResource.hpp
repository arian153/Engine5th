#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    enum class eShaderStage : int
    {
        IntegratedShader = -1,
        VertexShader,
        PixelShader,
        InvalidShader
    };

    class ShaderResource final : public Resource
    {
    public:
        explicit ShaderResource(const std::wstring& path);
        ~ShaderResource();

        void Initialize() override;
        void Shutdown() override;

        void SetStage(eShaderStage stage);

    private:
        bool         m_b_integrated_shader = false;
        eShaderStage m_stage;
    };
}

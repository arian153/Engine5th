#pragma once
#include "../Resource.hpp"
#include "../../../System/Graphics/Common/Shader/ShaderType.hpp"

namespace Engine5
{
    class ShaderResource final : public Resource
    {
    public:
        explicit ShaderResource(const std::wstring& path);
        ~ShaderResource();

        void Initialize() override;
        void Shutdown() override;

    private:
        bool         m_b_integrated_shader = false;
        eShaderStage m_stage;
    };
}

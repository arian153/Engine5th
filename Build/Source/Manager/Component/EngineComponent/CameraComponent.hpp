#pragma once
#include "..//Component.hpp"

namespace Engine5
{
    class CameraComponent final : public Component
    {
    public:
        CameraComponent();
        ~CameraComponent();

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
    private:

    };

}

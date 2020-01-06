#pragma once
#include "../Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace Engine5
{
    class ColliderComponent final : public Component
    {
    public:
        ColliderComponent();
        ~ColliderComponent();

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    private:
        ColliderSet* m_collider_set = nullptr;
    };
}

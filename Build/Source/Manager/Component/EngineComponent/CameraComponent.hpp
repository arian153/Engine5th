#pragma once
#include "..//Component.hpp"
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class CameraFactory final : public ComponentFactory
    {
        CameraFactory();
        ~CameraFactory();

        Component* Create(Object* owner) override;
        Component* Clone(Component* origin, Object* dest) override;
    };

    class CameraComponent final : public Component
    {
    public:
        ~CameraComponent();
        CameraComponent()                                      = delete;
        CameraComponent(const CameraComponent& rhs)            = delete;
        CameraComponent& operator=(const CameraComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        void Load() override;
        void Unload() override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class CameraFactory;

    private:
        explicit CameraComponent(Object* owner);
        void     Clone(CameraComponent* cloned);
    };
}

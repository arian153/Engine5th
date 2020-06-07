#pragma once
#include "../../../System/Core/Core.hpp"
#include "../../../System/Math/Math.hpp"
#include "../../../Manager/Component/GameComponent/LogicComponent.hpp"

namespace Game
{
    class ControllerComponent final : public Engine5::LogicComponent
    {
    public:
        ~ControllerComponent();
        ControllerComponent() = delete;
        ControllerComponent(const ControllerComponent& rhs) = delete;
        ControllerComponent& operator=(const ControllerComponent& rhs) = delete;

        void Initialize() override;
        void Update(Engine5::Real dt) override;
        void Shutdown() override;


    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class ControllerFactory;

    private:
        explicit ControllerComponent(Engine5::Object* owner);
        void     Clone(ControllerComponent* origin);
    };
}

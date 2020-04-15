#pragma once
#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace Engine5
{
    class Quaternion;
    class Vector3;
    class Transform;

    class LightComponent final : public Component
    {
    public:
        ~LightComponent();
        LightComponent() = delete;
        LightComponent(const LightComponent& rhs) = delete;
        LightComponent& operator=(const LightComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class LightFactory;

    private:
        explicit LightComponent(Object* owner);
        void     Clone(LightComponent* origin);

    private:
        Transform* m_transform = nullptr;
    };
}

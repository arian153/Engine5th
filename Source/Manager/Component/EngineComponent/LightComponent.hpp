#pragma once
#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"
#include "../../../System/Graphics/Common/Light/LightDef.hpp"

namespace Engine5
{
    class Light2;
    class Quaternion;
    class Vector3;
    class Transform;

    class LightComponent final : public Component
    {
    public:
        ~LightComponent();
        LightComponent()                                     = delete;
        LightComponent(const LightComponent& rhs)            = delete;
        LightComponent& operator=(const LightComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        void SetLightType(eLightType type);

        Light2* GetLight() const;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class LightFactory;
        friend class Light2;

    private:
        explicit LightComponent(Object* owner);
        void     Clone(LightComponent* origin);

    private:
        Light2*    m_light      = nullptr;
        Transform* m_transform  = nullptr;
        eLightType m_light_type = eLightType::DirectionalLight;
    };
}

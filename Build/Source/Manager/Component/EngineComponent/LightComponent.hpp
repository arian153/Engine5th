#pragma once
#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"
#include "../../../System/Graphics/Light/Light.hpp"
#include "../../../System/Graphics/Light/LightDef.hpp"

namespace Engine5
{
    class Quaternion;
    class Vector3;
    class Transform;
    class DirectionalLight;
    class PointLight;
    class SpotLight;
    class CapsuleLight;

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
        void BuildLight();

        DirectionalLight* GetDirectionalLight() const;
        PointLight* GetPointLight() const;
        SpotLight* GetSpotLight() const;
        CapsuleLight* GetCapsuleLight() const;
        Light* GetLight() const;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class LightFactory;
        friend class Light;

    private:
        explicit LightComponent(Object* owner);
        void     Clone(LightComponent* origin);

    private:
        Light*     m_light     = nullptr;
        Transform* m_transform = nullptr;
        eLightType m_light_type = eLightType::DirectionalLight;
    };
}

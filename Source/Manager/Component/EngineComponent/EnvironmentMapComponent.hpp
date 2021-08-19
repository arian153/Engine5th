#pragma once

#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace Engine5
{
    class EnvironmentMapComponent final : public Component
    {
    public:
        ~EnvironmentMapComponent();
        EnvironmentMapComponent()                                              = delete;
        EnvironmentMapComponent(const EnvironmentMapComponent& rhs)            = delete;
        EnvironmentMapComponent& operator=(const EnvironmentMapComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class EnvironmentMapFactory;
        friend class SkyBox;
        friend class SkyDome;

    private:
        explicit EnvironmentMapComponent(Object* owner);
        void     Clone(EnvironmentMapComponent* origin);

    private:
        SkyBox*  m_sky_box  = nullptr;
        SkyDome* m_sky_dome = nullptr;
    };
}

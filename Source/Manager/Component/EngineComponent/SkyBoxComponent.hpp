#pragma once

#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace Engine5
{
    class SkyBoxComponent final : public Component
    {
    public:
        ~SkyBoxComponent();
        SkyBoxComponent()                                      = delete;
        SkyBoxComponent(const SkyBoxComponent& rhs)            = delete;
        SkyBoxComponent& operator=(const SkyBoxComponent& rhs) = delete;

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
        friend class SkyBoxFactory;
        friend class SkyBox;

    private:
        explicit SkyBoxComponent(Object* owner);
        void     Clone(SkyBoxComponent* origin);

    private:
    };
}

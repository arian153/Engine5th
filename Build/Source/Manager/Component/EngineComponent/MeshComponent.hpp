#pragma once
#include "..//Component.hpp"

namespace Engine5
{
    class Quaternion;
    class Vector3;
    class Transform;

    class MeshComponent final : public Component
    {
    public:
        ~MeshComponent();
        MeshComponent()                                    = delete;
        MeshComponent(const MeshComponent& rhs)            = delete;
        MeshComponent& operator=(const MeshComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class MeshFactory;
        friend class Mesh;

    private:
        explicit MeshComponent(Object* owner);
        void     Clone(MeshComponent* origin);

    private:
        Mesh*      m_mesh      = nullptr;
        Transform* m_transform = nullptr;
    };
}

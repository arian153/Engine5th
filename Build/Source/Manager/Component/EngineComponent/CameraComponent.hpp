#pragma once
#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace Engine5
{
    class Quaternion;
    class Vector3;
    class Transform;
    class Camera;

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

        void SetPosition(const Vector3& position) const;
        void SetOrientation(const Quaternion& orientation) const;
        void Sync() const;
        void LookAt(const Vector3& position, const Vector3& target, const Vector3& up = Math::Vector3::Y_AXIS) const;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class CameraFactory;
        friend class Camera;

    private:
        explicit CameraComponent(Object* owner);
        void     Clone(CameraComponent* origin);

    private:
        Camera*    m_camera    = nullptr;
        Transform* m_transform = nullptr;
    };
}

#include "Camera.hpp"
#include "../../Math/Math.hpp"
#include "../../Math/Utility/MatrixUtility.hpp"
#include "Scene.hpp"
#include "../../../Manager/Component/EngineComponent/CameraComponent.hpp"

namespace Engine5
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::Initialize()
    {
    }

    void Camera::Update()
    {
        Vector3 up    = m_orientation.Rotate(Math::Vector3::Y_AXIS);
        Vector3 look  = m_orientation.Rotate(Math::Vector3::Z_AXIS) + m_position;
        m_view_matrix = Math::Matrix44::LookAt(m_position, look, up);
        m_view_matrix *= m_zoom;
        if (m_scene != nullptr)
        {
            m_scene->UpdateView();
        }
    }

    void Camera::Shutdown() const
    {
        if (m_component != nullptr)
        {
            m_component->m_camera = nullptr;
        }
    }

    void Camera::SetPosition(const Vector3& position)
    {
        m_position = position;
        Update();
    }

    void Camera::SetOrientation(const Quaternion& orientation)
    {
        m_orientation = orientation;
        m_basis       = Basis();
        m_basis.Rotate(m_orientation);
        Update();
    }

    void Camera::SetScene(Scene* scene)
    {
        if (scene != nullptr)
        {
            m_scene = scene;
        }
    }

    Vector3 Camera::GetPosition() const
    {
        return m_position;
    }

    Quaternion Camera::GetOrientation() const
    {
        return m_orientation;
    }

    Matrix44 Camera::GetViewMatrix() const
    {
        return m_view_matrix;
    }

    void Camera::LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
    {
        m_view_matrix            = Math::Matrix44::LookAt(position, target, up);
        Matrix44 view_matrix     = m_view_matrix;
        view_matrix              = Math::Matrix44::Translation(-position) * view_matrix;
        Matrix33 rotation_matrix = Math::Matrix33::Rotation(view_matrix);
        m_orientation            = rotation_matrix.ToQuaternion();
        m_position               = position;
        m_basis                  = Basis();
        m_basis.Rotate(m_orientation);
        m_view_matrix *= m_zoom;
    }

    Basis Camera::GetBasis() const
    {
        return m_basis;
    }

    void Camera::SyncToTransform(Transform* transform) const
    {
        if (transform != nullptr)
        {
            transform->position    = m_position;
            transform->orientation = m_orientation;
            transform->scale       = Vector3(m_zoom, m_zoom, m_zoom);
        }
    }

    void Camera::SyncFromTransform(Transform* transform)
    {
        if (transform != nullptr)
        {
            m_position    = transform->position;
            m_orientation = transform->orientation;
            m_zoom        = transform->scale.x;
            m_basis = Basis();
            m_basis.Rotate(m_orientation);
            Update();
        }
    }
}

#include "Camera.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../../Math/Math.hpp"
#include "../Renderer/DX11/ConverterDX11.hpp"
#include "../../Math/Utility/MatrixUtility.hpp"

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

    void Camera::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        SyncFromTransform();
    }

    void Camera::Shutdown()
    {
    }

    void Camera::SetPosition(const Vector3& position)
    {
        m_position = position;
        UpdateCamera();
        SyncToTransform();
    }

    void Camera::SetOrientation(const Quaternion& orientation)
    {
        m_orientation = orientation;
        UpdateCamera();
        SyncToTransform();
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
        m_position               = position;
        Matrix44 view_matrix     = m_view_matrix;
        view_matrix              = Math::Matrix44::Translation(-position) * view_matrix;
        Matrix33 rotation_matrix = Math::Matrix33::Rotation(view_matrix);
        m_orientation            = rotation_matrix.ToQuaternion();
        m_basis                  = Basis();
        m_basis.Rotate(m_orientation);
        SyncToTransform();
    }

    Basis Camera::GetBasis() const
    {
        return m_basis;
    }

    void Camera::UpdateCamera()
    {
        Vector3 up    = m_orientation.Rotate(Math::Vector3::Y_AXIS);
        Vector3 look  = m_orientation.Rotate(Math::Vector3::Z_AXIS) + m_position;
        m_view_matrix = Math::Matrix44::LookAt(m_position, look, up);
    }

    void Camera::SyncFromTransform()
    {
    }

    void Camera::SyncToTransform()
    {
    }
}

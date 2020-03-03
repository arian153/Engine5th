#include "Camera.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../../Math/Math.hpp"
#include "../Renderer/DX11/ConverterDX11.hpp"

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
        DirectX::XMVECTOR pos_vector    = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);
        DirectX::XMVECTOR target_vector = DirectX::XMVectorSet(target.x, target.y, target.z, 0.0f);
        DirectX::XMVECTOR up_vector     = DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f);
        m_view_matrix                   = DirectX::XMMatrixLookAtLH(pos_vector, target_vector, up_vector);
        m_position                      = position;
        DirectX::XMMATRIX view_matrix   = m_view_matrix;
        view_matrix                     = DirectX::XMMatrixTranslation(-position.x, -position.y, -position.z) * view_matrix;
        DirectX::XMFLOAT4X4 dx_rotation_matrix;
        DirectX::XMStoreFloat4x4(&dx_rotation_matrix, view_matrix);
        Matrix33 rotation_matrix;
        rotation_matrix.data[0] = dx_rotation_matrix._11;
        rotation_matrix.data[1] = dx_rotation_matrix._12;
        rotation_matrix.data[2] = dx_rotation_matrix._13;
        rotation_matrix.data[3] = dx_rotation_matrix._21;
        rotation_matrix.data[4] = dx_rotation_matrix._22;
        rotation_matrix.data[5] = dx_rotation_matrix._23;
        rotation_matrix.data[6] = dx_rotation_matrix._31;
        rotation_matrix.data[7] = dx_rotation_matrix._32;
        rotation_matrix.data[8] = dx_rotation_matrix._33;
        m_orientation           = rotation_matrix.ToQuaternion();
        m_basis                 = Basis();
        m_basis.Rotate(m_orientation);
        SyncToTransform();
    }

    Basis Camera::GetBasis() const
    {
        return m_basis;
    }

    void Camera::UpdateCamera()
    {
        DirectX::XMVECTOR up_vector       = ConverterDX11::ToXMVector(Math::Vector3::Y_AXIS);
        DirectX::XMVECTOR pos_vector      = ConverterDX11::ToXMVector(m_position);
        DirectX::XMVECTOR look_vector     = ConverterDX11::ToXMVector(Math::Vector3::Z_AXIS);
        DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationQuaternion(ConverterDX11::ToXMVector(m_orientation));

        // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
        look_vector = DirectX::XMVector3TransformCoord(look_vector, rotation_matrix);
        up_vector   = DirectX::XMVector3TransformCoord(up_vector, rotation_matrix);

        // Translate the rotated camera position to the location of the viewer.
        look_vector = DirectX::XMVectorAdd(pos_vector, look_vector);

        // Finally create the view matrix from the three updated vectors.
        m_view_matrix = DirectX::XMMatrixLookAtLH(pos_vector, look_vector, up_vector);
    }

    void Camera::SyncFromTransform()
    {
    }

    void Camera::SyncToTransform()
    {
    }
}

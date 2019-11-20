#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Quaternion.hpp"
#include <directxmath.h>
#include "../../Math/Structure/Basis.hpp"

namespace Engine5
{
    class Camera
    {
    public:
        Camera();
        ~Camera();

        void Initialize();
        void Update(Real dt);
        void Shutdown();
        void SetPosition(const Vector3& position);
        void SetOrientation(const Quaternion& orientation);
        void LookAt(const Vector3& position, const Vector3& target, const Vector3& up = Vector3::AxisY());

        Vector3           GetPosition() const;
        Quaternion        GetOrientation() const;
        DirectX::XMMATRIX GetViewMatrix() const;
        Basis             GetBasis() const;

    private:
        void UpdateCamera();
        void SyncFromTransform();
        void SyncToTransform();

    private:
        DirectX::XMMATRIX m_view_matrix = DirectX::XMMatrixIdentity();
        Vector3           m_position;
        Quaternion        m_orientation;
        Real              m_zoom = 1.0f;
        Basis             m_basis;
    };
}

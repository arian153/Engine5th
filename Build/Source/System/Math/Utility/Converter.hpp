#pragma once

#include <directxmath.h>

namespace Engine5
{
    class Vector2;
    class Vector3;
    class Vector4;
    class Quaternion;
    class Matrix33;
    class Matrix44;

    namespace Converter
    {
        Vector3 ToVector3(const DirectX::XMVECTOR& xmvector);
        Vector3 ToVector3(const DirectX::XMFLOAT3& xmfloat3);

        Vector4 ToVector4(const DirectX::XMVECTOR& xmvector);
        Vector4 ToVector4(const DirectX::XMFLOAT4& xmfloat4);

        Quaternion ToQuaternion(const DirectX::XMVECTOR& xmvector);
        Quaternion ToQuaternion(const DirectX::XMFLOAT4& xmfloat4);


        DirectX::XMVECTOR ToXMVector(const Vector3& vector);
        DirectX::XMVECTOR ToXMVector(const Vector4& vector);
        DirectX::XMVECTOR ToXMVector(const Quaternion& quaternion);

        DirectX::XMFLOAT2 ToXMFloat2(const Vector2& vector);
        DirectX::XMFLOAT3 ToXMFloat3(const Vector3& vector);
        DirectX::XMFLOAT4 ToXMFloat4(const Vector4& vector);
        DirectX::XMFLOAT4 ToXMFloat4(const Quaternion& quaternion);
    }
}

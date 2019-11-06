#include "Converter.hpp"
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Vector4.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Matrix33.hpp"
#include "../Algebra/Matrix44.hpp"
#include "../Algebra/Vector2.hpp"


namespace Engine5
{
    namespace Converter
    {
        Vector3 ToVector3(const DirectX::XMVECTOR& xmvector)
        {
            return Vector3(xmvector.m128_f32[0], xmvector.m128_f32[1], xmvector.m128_f32[2]);
        }

        Vector3 ToVector3(const DirectX::XMFLOAT3& xmfloat3)
        {
            return Vector3(xmfloat3.x, xmfloat3.y, xmfloat3.z);
        }

        Vector4 ToVector4(const DirectX::XMVECTOR& xmvector)
        {
            return Vector4(xmvector.m128_f32[0], xmvector.m128_f32[1], xmvector.m128_f32[2], xmvector.m128_f32[3]);
        }

        Vector4 ToVector4(const DirectX::XMFLOAT4& xmfloat4)
        {
            return Vector4(xmfloat4.x, xmfloat4.y, xmfloat4.z, xmfloat4.w);
        }

        Quaternion ToQuaternion(const DirectX::XMVECTOR& xmvector)
        {
            return Quaternion(xmvector.m128_f32[3], xmvector.m128_f32[0], xmvector.m128_f32[1], xmvector.m128_f32[2]);
        }

        Quaternion ToQuaternion(const DirectX::XMFLOAT4& xmfloat4)
        {
            return Quaternion(xmfloat4.w, xmfloat4.x, xmfloat4.y, xmfloat4.z);
        }

        DirectX::XMVECTOR ToXMVector(const Vector3& vector)
        {
            DirectX::XMFLOAT3 float3(vector.x, vector.y, vector.z);
            return DirectX::XMLoadFloat3(&float3);
        }

        DirectX::XMVECTOR ToXMVector(const Vector4& vector)
        {
            DirectX::XMFLOAT4 float4(vector.x, vector.y, vector.z, vector.w);
            return DirectX::XMLoadFloat4(&float4);
        }

        DirectX::XMVECTOR ToXMVector(const Quaternion& quaternion)
        {
            DirectX::XMFLOAT4 float4(quaternion.i, quaternion.j, quaternion.k, quaternion.r);
            return DirectX::XMLoadFloat4(&float4);
        }

        DirectX::XMFLOAT2 ToXMFloat2(const Vector2& vector)
        {
            return DirectX::XMFLOAT2(vector.x, vector.y);
        }

        DirectX::XMFLOAT3 ToXMFloat3(const Vector3& vector)
        {
            return DirectX::XMFLOAT3(vector.x, vector.y, vector.z);
        }

        DirectX::XMFLOAT4 ToXMFloat4(const Vector4& vector)
        {
            return DirectX::XMFLOAT4(vector.x, vector.y, vector.z, vector.w);
        }

        DirectX::XMFLOAT4 ToXMFloat4(const Quaternion& quaternion)
        {
            return DirectX::XMFLOAT4(quaternion.i, quaternion.j, quaternion.k, quaternion.r);
        }
    }
}

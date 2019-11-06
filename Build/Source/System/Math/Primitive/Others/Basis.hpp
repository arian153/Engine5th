#pragma once
#include "../../Algebra/Vector3.hpp"
#include "../../Algebra/Quaternion.hpp"

namespace Engine5
{
    class Basis
    {
    public:
        explicit Basis(const Vector3& a = Vector3::AxisX(), const Vector3& b = Vector3::AxisY(), const Vector3& c = Vector3::AxisZ());
        Basis(const Basis& rhs);
        Basis& operator=(const Basis& rhs);
        ~Basis();

        void CalculateBasis(const Vector3& normal);
        void CalculateBasisApprox(const Vector3& normal);
        void CalculateBasis(const Vector3& a, const Vector3& b);
        void Rotate(const Quaternion& orientation);
        void Normalize();

    public:
        Vector3 i = Vector3::AxisX();
        Vector3 j = Vector3::AxisY();
        Vector3 k = Vector3::AxisZ();
    };
}
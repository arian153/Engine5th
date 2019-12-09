//**************************************************************//
//  _______ .__   __.   _______  __  .__   __.  _______  _____  //
// |   ____||  \ |  |  /  _____||  | |  \ |  | |   ____|| ____| //
// |  |__   |   \|  | |  |  __  |  | |   \|  | |  |__   | |__   //
// |   __|  |  . `  | |  | |_ | |  | |  . `  | |   __|  |___ \  //
// |  |____ |  |\   | |  |__| | |  | |  |\   | |  |____  ___) | //
// |_______||__| \__|  \______| |__| |__| \__| |_______||____/  //
//                                                              //
//**************************************************************//

#pragma once
#include "../Utility/MathDef.hpp"

namespace Engine5
{
    class Vector4;
    class Vector2;
    class Matrix33;

    class Vector3
    {
    public:
        explicit Vector3(Real x = 0.f, Real y = 0.f, Real z = 0.f);
        explicit Vector3(Real arr[3]);
        explicit Vector3(const Vector2& rhs);
        explicit Vector3(const Vector4& rhs);
        Vector3(const Vector3& rhs);

        ~Vector3();

        void Set(Real x = 0.0f, Real y = 0.0f, Real z = 0.0f);
        void SetZero();
        void SetInverse();
        void SetNegate();
        void SetNormalize();
        void SetHalf();
        void SetClean();
        void SetProjection(const Vector3& a, const Vector3& b);

        Real Length() const;
        Real LengthSquared() const;

        Real    DistanceTo(const Vector3& rhs) const;
        Real    DistanceSquaredTo(const Vector3& rhs) const;
        Vector3 ProjectionTo(const Vector3& rhs) const;
        Vector3 ProjectionFrom(const Vector3& rhs) const;
        Vector3 Unit() const;
        Vector3 Half() const;
        Vector3 Inverse() const;
        Vector3 Negate() const;

        Real     DotProduct(const Vector3& rhs) const;
        Vector3  CrossProduct(const Vector3& rhs) const;
        Matrix33 OuterProduct(const Vector3& rhs) const;
        Vector3  HadamardProduct(const Vector3& rhs) const;
        Vector3  CrossProductTwice(const Vector3& rhs) const;

        bool IsValid() const;
        bool IsZero() const;
        bool IsEqual(const Vector3& rhs) const;
        bool IsNotEqual(const Vector3& rhs) const;

    public:
        friend Real     DotProduct(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  CrossProduct(const Vector3& vec1, const Vector3& vec2);
        friend Matrix33 OuterProduct(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  HadamardProduct(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  CrossProductTwice(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  Projection(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  LinearInterpolation(const Vector3& start, const Vector3& end, Real t);
        friend Real     Radian(const Vector3& a, const Vector3& b);

    public:
        bool                 operator ==(const Vector3& rhs) const;
        bool                 operator !=(const Vector3& rhs) const;
        Vector3              operator -() const;
        Vector3&             operator =(const Vector2& rhs);
        Vector3&             operator =(const Vector3& rhs);
        Vector3&             operator =(const Vector4& rhs);
        Vector3&             operator =(Real rhs);
        Vector3&             operator +=(const Vector3& rhs);
        Vector3&             operator +=(Real real);
        Vector3&             operator -=(const Vector3& rhs);
        Vector3&             operator -=(Real real);
        Vector3&             operator *=(Real real);
        Vector3&             operator /=(Real real);
        Vector3              operator +(const Vector3& rhs) const;
        Vector3              operator +(Real real) const;
        Vector3              operator -(const Vector3& rhs) const;
        Vector3              operator -(Real real) const;
        Vector3              operator *(Real real) const;
        Vector3              operator /(Real real) const;
        friend std::ostream& operator<<(std::ostream& os, const Vector3& rhs);
        friend Vector3       operator*(Real real, const Vector3& vector);
        Vector3&             operator ++();
        Vector3              operator ++(int);
        Vector3&             operator --();
        Vector3              operator --(int);
        Real                 operator[](size_t i) const;
        Real&                operator[](size_t i);
        Real                 operator *(const Vector3& rhs) const;

    public:
        static Vector3 AxisX();
        static Vector3 AxisY();
        static Vector3 AxisZ();
        static Vector3 Origin();

    public:
        Real x, y, z;
    };
}

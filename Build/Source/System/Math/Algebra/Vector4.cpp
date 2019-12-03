//**************************************************************//
//  _______ .__   __.   _______  __  .__   __.  _______  _____  //
// |   ____||  \ |  |  /  _____||  | |  \ |  | |   ____|| ____| //
// |  |__   |   \|  | |  |  __  |  | |   \|  | |  |__   | |__   //
// |   __|  |  . `  | |  | |_ | |  | |  . `  | |   __|  |___ \  //
// |  |____ |  |\   | |  |__| | |  | |  |\   | |  |____  ___) | //
// |_______||__| \__|  \______| |__| |__| \__| |_______||____/  //
//                                                              //
//**************************************************************//

#include "Vector4.hpp"
#include "..//Utility/Utility.hpp"
#include <ostream>
#include "Matrix44.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

namespace Engine5
{
    Vector4::Vector4(Real x, Real y, Real z, Real w)
        : x(x), y(y), z(z), w(w)
    {
    }

    Vector4::Vector4(Real arr[4])
        : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])
    {
    }

    Vector4::Vector4(const Vector2& rhs)
        : x(rhs.x), y(rhs.y), z(0.0f), w(0.0f)
    {
    }

    Vector4::Vector4(const Vector3& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z), w(0.0f)
    {
    }

    Vector4::Vector4(const Vector4& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
    {
    }

    Vector4::~Vector4()
    {
    }

    void Vector4::Set(Real _x, Real _y, Real _z, Real _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    void Vector4::SetZero()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    void Vector4::SetInverse()
    {
        x = 1.0f / x;
        y = 1.0f / y;
        z = 1.0f / z;
        w = 1.0f / w;
    }

    void Vector4::SetNegate()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
    }

    void Vector4::SetNormalize()
    {
        Real length = sqrt(x * x + y * y + z * z + w * w);
        if (length > 0.0f)
        {
            (*this) *= (1.0f / length);
        }
    }

    void Vector4::SetHalf()
    {
        this->x *= 0.5f;
        this->y *= 0.5f;
        this->z *= 0.5f;
        this->w *= 0.5f;
    }

    void Vector4::SetClean()
    {
        if (Utility::IsZero(x))
            x = 0.0f;
        if (Utility::IsZero(y))
            y = 0.0f;
        if (Utility::IsZero(z))
            z = 0.0f;
        if (Utility::IsZero(w))
            w = 0.0f;
    }

    void Vector4::SetProjection(const Vector4& a, const Vector4& b)
    {
        Real multiplier = (a.DotProduct(b)) / (b.DotProduct(b));
        this->x         = b.x * multiplier;
        this->y         = b.y * multiplier;
        this->z         = b.z * multiplier;
        this->w         = b.w * multiplier;
    }

    Real Vector4::Length() const
    {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

    Real Vector4::LengthSquared() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    Real Vector4::DistanceTo(const Vector4& rhs) const
    {
        Real _x = rhs.x - this->x;
        Real _y = rhs.y - this->y;
        Real _z = rhs.z - this->z;
        Real _w = rhs.w - this->w;
        return sqrtf(_x * _x + _y * _y + _z * _z + _w * _w);
    }

    Real Vector4::DistanceSquaredTo(const Vector4& rhs) const
    {
        Real _x = rhs.x - this->x;
        Real _y = rhs.y - this->y;
        Real _z = rhs.z - this->z;
        Real _w = rhs.w - this->w;
        return (_x * _x + _y * _y + _z * _z + _w * _w);
    }

    Vector4 Vector4::ProjectionTo(const Vector4& rhs) const
    {
        Vector4    result     = rhs;
        const Real multiplier = ((*this).DotProduct(rhs)) / (rhs.DotProduct(rhs));
        result *= multiplier;
        return result;
    }

    Vector4 Vector4::ProjectionFrom(const Vector4& rhs) const
    {
        Vector4 result     = *this;
        Real    multiplier = (rhs.DotProduct(*this)) / ((*this).DotProduct(*this));
        result *= multiplier;
        return result;
    }

    Vector4 Vector4::Unit() const
    {
        Vector4 result = *this;
        result.SetNormalize();
        return result;
    }

    Vector4 Vector4::Half() const
    {
        Vector4 result = *this;
        result.SetHalf();
        return result;
    }

    Real Vector4::DotProduct(const Vector4& rhs) const
    {
        return (x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
    }

    Vector4 Vector4::CrossProduct(const Vector4& rhs) const
    {
        return Vector4(
                       y * rhs.z - z * rhs.y,
                       -x * rhs.z + z * rhs.x,
                       x * rhs.y - y * rhs.x,
                       1.0f);
    }

    Matrix44 Vector4::OuterProduct(const Vector4& rhs) const
    {
        return Matrix44(
                        (this->x * rhs.x), (this->x * rhs.y), (this->x * rhs.z), (this->x * rhs.w),
                        (this->y * rhs.x), (this->y * rhs.y), (this->y * rhs.z), (this->y * rhs.w),
                        (this->z * rhs.x), (this->z * rhs.y), (this->z * rhs.z), (this->z * rhs.w),
                        (this->w * rhs.x), (this->w * rhs.y), (this->w * rhs.z), (this->w * rhs.w)
                       );
    }

    Vector4 Vector4::HadamardProduct(const Vector4& rhs) const
    {
        return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    bool Vector4::IsValid() const
    {
        return Utility::IsValid(x) && Utility::IsValid(y) && Utility::IsValid(z) && Utility::IsValid(w);
    }

    bool Vector4::IsZero() const
    {
        return Utility::IsZero(x) && Utility::IsZero(y) && Utility::IsZero(z) && Utility::IsZero(w);
    }

    bool Vector4::IsEqual(const Vector4& rhs) const
    {
        return false;
    }

    bool Vector4::IsNotEqual(const Vector4& rhs) const
    {
        return false;
    }

    bool Vector4::operator==(const Vector4& rhs) const
    {
        return (Utility::IsEqual(x, rhs.x)
            && Utility::IsEqual(y, rhs.y)
            && Utility::IsEqual(z, rhs.z)
            && Utility::IsEqual(z, rhs.w));
    }

    bool Vector4::operator!=(const Vector4& rhs) const
    {
        return (Utility::IsNotEqual(x, rhs.x)
            || Utility::IsNotEqual(y, rhs.y)
            || Utility::IsNotEqual(z, rhs.z)
            || Utility::IsNotEqual(z, rhs.w));
    }

    Vector4 Vector4::operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    Vector4& Vector4::operator=(const Vector2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector4& Vector4::operator=(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector4& Vector4::operator=(const Vector4& rhs)
    {
        if (this != &rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
        }
        return *this;
    }

    Vector4& Vector4::operator=(Real rhs)
    {
        x = rhs;
        y = rhs;
        z = rhs;
        w = rhs;
        return *this;
    }

    Vector4& Vector4::operator+=(const Vector4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4& Vector4::operator+=(Real real)
    {
        x += real;
        y += real;
        z += real;
        w += real;
        return *this;
    }

    Vector4& Vector4::operator-=(const Vector4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4& Vector4::operator-=(Real real)
    {
        x -= real;
        y -= real;
        z -= real;
        w -= real;
        return *this;
    }

    Vector4& Vector4::operator*=(Real real)
    {
        x *= real;
        y *= real;
        z *= real;
        w *= real;
        return *this;
    }

    Vector4& Vector4::operator/=(Real real)
    {
        x /= real;
        y /= real;
        z /= real;
        w /= real;
        return *this;
    }

    Vector4 Vector4::operator+(const Vector4& rhs) const
    {
        return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vector4 Vector4::operator+(Real real) const
    {
        return Vector4(x + real, y + real, z + real, w + real);
    }

    Vector4 Vector4::operator-(const Vector4& rhs) const
    {
        return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vector4 Vector4::operator-(Real real) const
    {
        return Vector4(x - real, y - real, z - real, w - real);
    }

    Vector4 Vector4::operator*(Real real) const
    {
        return Vector4(x * real, y * real, z * real, w * real);
    }

    Vector4 Vector4::operator/(Real real) const
    {
        return Vector4(x / real, y / real, z / real, w / real);
    }

    Vector4& Vector4::operator++()
    {
        ++x;
        ++y;
        ++z;
        ++w;
        return *this;
    }

    Vector4 Vector4::operator++(int)
    {
        Vector4 result(*this);
        ++(*this);
        return result;
    }

    Vector4& Vector4::operator--()
    {
        --x;
        --y;
        --z;
        --w;
        return *this;
    }

    Vector4 Vector4::operator--(int)
    {
        Vector4 result(*this);
        --(*this);
        return result;
    }

    Real Vector4::operator[](size_t i) const
    {
        return (&x)[i];
    }

    Real& Vector4::operator[](size_t i)
    {
        return (&x)[i];
    }

    Real DotProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.DotProduct(vec2);
    }

    Vector4 CrossProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.CrossProduct(vec2);
    }

    Matrix44 OuterProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.OuterProduct(vec2);
    }

    Vector4 HadamardProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.HadamardProduct(vec2);
    }

    Vector4 Projection(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.ProjectionTo(vec2);
    }

    Vector4 LinearInterpolation(const Vector4& start, const Vector4& end, Real t)
    {
        Vector4 result;
        result.x = (1.0f - t) * start.x + t * end.x;
        result.y = (1.0f - t) * start.y + t * end.y;
        result.z = (1.0f - t) * start.z + t * end.z;
        result.w = (1.0f - t) * start.w + t * end.w;
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Vector4& rhs)
    {
        os << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << "]";
        return os;
    }

    Vector4 operator*(Real real, const Vector4& vector)
    {
        return Vector4(vector.x * real, vector.y * real, vector.z * real, vector.w * real);
    }
}

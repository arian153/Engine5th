//**************************************************************//
//  _______ .__   __.   _______  __  .__   __.  _______  _____  //
// |   ____||  \ |  |  /  _____||  | |  \ |  | |   ____|| ____| //
// |  |__   |   \|  | |  |  __  |  | |   \|  | |  |__   | |__   //
// |   __|  |  . `  | |  | |_ | |  | |  . `  | |   __|  |___ \  //
// |  |____ |  |\   | |  |__| | |  | |  |\   | |  |____  ___) | //
// |_______||__| \__|  \______| |__| |__| \__| |_______||____/  //
//                                                              //
//**************************************************************//

#include "Vector2.hpp"
#include "..//Utility/Utility.hpp"
#include <ostream>
#include "Matrix22.hpp"

namespace Engine5
{
    Vector2::Vector2(Real x, Real y)
        : x(x), y(y)
    {
    }

    Vector2::Vector2(Real arr[2])
        : x(arr[0]), y(arr[1])
    {
    }

    Vector2::Vector2(const Vector2& rhs)
        : x(rhs.x), y(rhs.y)
    {
    }

    Vector2::~Vector2()
    {
    }

    void Vector2::Set(Real _x, Real _y)
    {
        x = _x;
        y = _y;
    }

    void Vector2::SetZero()
    {
        x = 0.0f;
        y = 0.0f;
    }

    void Vector2::SetInverse()
    {
        x = 1.0f / x;
        y = 1.0f / y;
    }

    void Vector2::SetNegate()
    {
        x = -x;
        y = -y;
    }

    void Vector2::SetNormalize()
    {
        Real length = sqrtf(x * x + y * y);
        if (length > 0.f)
        {
            (*this) *= (1.f / length);
        }
    }

    void Vector2::SetHalf()
    {
        this->x *= 0.5f;
        this->y *= 0.5f;
    }

    void Vector2::SetClean()
    {
        if (Utility::IsZero(x))
            x = 0.0f;
        if (Utility::IsZero(y))
            y = 0.0f;
    }

    void Vector2::SetProjection(const Vector2& a, const Vector2& b)
    {
        Real multiplier = (a.DotProduct(b)) / (b.DotProduct(b));
        this->x         = b.x * multiplier;
        this->y         = b.y * multiplier;
    }

    Real Vector2::Length() const
    {
        return sqrtf(x * x + y * y);
    }

    Real Vector2::LengthSquared() const
    {
        return (x * x + y * y);
    }

    Real Vector2::DistanceTo(const Vector2& rhs) const
    {
        Real _x = rhs.x - this->x;
        Real _y = rhs.y - this->y;
        return sqrtf(_x * _x + _y * _y);
    }

    Real Vector2::DistanceSquaredTo(const Vector2& rhs) const
    {
        Real _x = rhs.x - this->x;
        Real _y = rhs.y - this->y;
        return (_x * _x + _y * _y);
    }

    Vector2 Vector2::ProjectionTo(const Vector2& rhs) const
    {
        Vector2 result     = rhs;
        Real    multiplier = ((*this).DotProduct(rhs)) / (rhs.DotProduct(rhs));
        result *= multiplier;
        return result;
    }

    Vector2 Vector2::ProjectionFrom(const Vector2& rhs) const
    {
        Vector2 result     = *this;
        Real    multiplier = (rhs.DotProduct(*this)) / ((*this).DotProduct(*this));
        result *= multiplier;
        return result;
    }

    Vector2 Vector2::Unit() const
    {
        Vector2 result = *this;
        result.SetNormalize();
        return result;
    }

    Vector2 Vector2::Half() const
    {
        Vector2 result = *this;
        result.SetHalf();
        return result;
    }

    Real Vector2::DotProduct(const Vector2& rhs) const
    {
        return (x * rhs.x + y * rhs.y);
    }

    Real Vector2::CrossProduct(const Vector2& rhs) const
    {
        return (x * rhs.y - y * rhs.x);
    }

    Vector2 Vector2::CrossProduct(const Real& rhs) const
    {
        return Vector2(rhs * y, -rhs * x);
    }

    Matrix22 Vector2::OuterProduct(const Vector2& rhs) const
    {
        return Matrix22(
                        this->x * rhs.x, this->x * rhs.y,
                        this->y * rhs.x, this->y * rhs.y);
    }

    Vector2 Vector2::HadamardProduct(const Vector2& rhs) const
    {
        return Vector2(x * rhs.x, y * rhs.y);
    }

    bool Vector2::IsValid() const
    {
        return Utility::IsValid(x) && Utility::IsValid(y);
    }

    bool Vector2::IsZero() const
    {
        return Utility::IsZero(x) && Utility::IsZero(y);
    }

    bool Vector2::IsEqual(const Vector2& rhs) const
    {
        return Utility::IsEqual(x, rhs.x) && Utility::IsEqual(y, rhs.y);
    }

    bool Vector2::IsNotEqual(const Vector2& rhs) const
    {
        return Utility::IsNotEqual(x, rhs.x) || Utility::IsNotEqual(y, rhs.y);
    }

    bool Vector2::operator==(const Vector2& rhs) const
    {
        return Utility::IsEqual(x, rhs.x) && Utility::IsEqual(y, rhs.y);
    }

    bool Vector2::operator!=(const Vector2& rhs) const
    {
        return Utility::IsNotEqual(x, rhs.x) || Utility::IsNotEqual(y, rhs.y);
    }

    Vector2 Vector2::operator-() const
    {
        return Vector2(-x, -y);
    }

    Vector2& Vector2::operator=(const Vector2& rhs)
    {
        if (this != &rhs)
        {
            x = rhs.x;
            y = rhs.y;
        }
        return *this;
    }

    Vector2& Vector2::operator=(Real rhs)
    {
        x = rhs;
        y = rhs;
        return *this;
    }

    Vector2& Vector2::operator+=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2& Vector2::operator+=(Real real)
    {
        x += real;
        y += real;
        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2& Vector2::operator-=(Real real)
    {
        x -= real;
        y -= real;
        return *this;
    }

    Vector2& Vector2::operator*=(Real real)
    {
        x *= real;
        y *= real;
        return *this;
    }

    Vector2& Vector2::operator/=(Real real)
    {
        x /= real;
        y /= real;
        return *this;
    }

    Vector2 Vector2::operator+(const Vector2& rhs) const
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }

    Vector2 Vector2::operator+(Real real) const
    {
        return Vector2(x + real, y + real);
    }

    Vector2 Vector2::operator-(const Vector2& rhs) const
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }

    Vector2 Vector2::operator-(Real real) const
    {
        return Vector2(x - real, y - real);
    }

    Vector2 Vector2::operator*(Real real) const
    {
        return Vector2(x * real, y * real);
    }

    Vector2 Vector2::operator/(Real real) const
    {
        return Vector2(x / real, y / real);
    }

    Vector2& Vector2::operator++()
    {
        ++x;
        ++y;
        return *this;
    }

    Vector2 Vector2::operator++(int)
    {
        Vector2 result(*this);
        ++(*this);
        return result;
    }

    Vector2& Vector2::operator--()
    {
        --x;
        --y;
        return *this;
    }

    Vector2 Vector2::operator--(int)
    {
        Vector2 result(*this);
        --(*this);
        return result;
    }

    Real Vector2::operator[](size_t i) const
    {
        return (&x)[i];
    }

    Real& Vector2::operator[](size_t i)
    {
        return (&x)[i];
    }

    Real DotProduct(const Vector2& vec1, const Vector2& vec2)
    {
        return vec1.DotProduct(vec2);
    }

    Real CrossProduct(const Vector2& vec1, const Vector2& vec2)
    {
        return vec1.CrossProduct(vec2);
    }

    Vector2 CrossProduct(Real vec1, const Vector2& vec2)
    {
        return Vector2(-vec1 * vec2.y, vec1 * vec2.x);
    }

    Vector2 CrossProduct(const Vector2& vec1, Real vec2)
    {
        return vec1.CrossProduct(vec2);
    }

    Matrix22 OuterProduct(const Vector2& vec1, const Vector2& vec2)
    {
        return vec1.OuterProduct(vec2);
    }

    Vector2 HadamardProduct(const Vector2& vec1, const Vector2& vec2)
    {
        return vec1.HadamardProduct(vec2);
    }

    Vector2 Projection(const Vector2& vec1, const Vector2& vec2)
    {
        return vec1.ProjectionTo(vec2);
    }

    Vector2 LinearInterpolation(const Vector2& start, const Vector2& end, Real t)
    {
        Vector2 result;
        result.x = (1.0f - t) * start.x + t * end.x;
        result.y = (1.0f - t) * start.y + t * end.y;
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Vector2& rhs)
    {
        os << "[" << rhs.x << ", " << rhs.y << "]";
        return os;
    }

    Vector2 operator*(Real real, const Vector2& vector)
    {
        return Vector2(vector.x * real, vector.y * real);
    }
}

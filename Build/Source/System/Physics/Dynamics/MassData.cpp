#include "MassData.hpp"
#include "../../Math/Algebra/Quaternion.hpp"

namespace Engine5
{
    MassData::MassData()
    {
    }

    MassData::~MassData()
    {
    }

    MassData& MassData::operator=(const MassData& rhs)
    {
        if (this != &rhs)
        {
            mass                         = rhs.mass;
            inverse_mass                 = rhs.inverse_mass;
            local_centroid               = rhs.local_centroid;
            local_inertia_tensor         = rhs.local_inertia_tensor;
            local_inverse_inertia_tensor = rhs.local_inverse_inertia_tensor;
        }
        return *this;
    }

    Matrix33 MassData::TranslateInertia(const Matrix33& input, Real mass, const Vector3& translation)
    {
        Real     r_dot_r = translation.DotProduct(translation);
        Matrix33 r_out_r = translation.OuterProduct(translation);
        return input + mass * (r_dot_r * Matrix33::Identity() - r_out_r);
    }

    Matrix33 MassData::RotateInertia(const Matrix33& input, const Quaternion& orientation)
    {
        return orientation * input * (orientation.Inverse());
    }
}

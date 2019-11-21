#include "MassData.hpp"

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
}

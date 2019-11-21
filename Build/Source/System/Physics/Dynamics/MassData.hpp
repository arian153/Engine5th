#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Matrix33.hpp"


namespace Engine5
{
    class MassData
    {
    public:
        MassData();
        ~MassData();

        MassData& operator=(const MassData& rhs);

    public:
        //mass
        Real mass = 1.0f;
        Real inverse_mass = 1.0f;

        //center of mass
        Vector3 local_centroid;

        //inertia
        Matrix33 local_inertia_tensor;
        Matrix33 local_inverse_inertia_tensor;
    };
}

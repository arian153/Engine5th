#pragma once
#include "../../Math/Algebra/Matrix44.hpp"

namespace Engine5
{
    class MatrixGenerator
    {
    public:
        MatrixGenerator();
        ~MatrixGenerator();

        Matrix44 ProjectionMatrix(Real screen_aspect, Real field_of_view, Real far_plane, Real near_plane) const;

    private:

    };

   }

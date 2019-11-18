#include "MatrixGenerator.hpp"

namespace Engine5
{
    MatrixGenerator::MatrixGenerator()
    {
    }

    MatrixGenerator::~MatrixGenerator()
    {
    }

    Matrix44 MatrixGenerator::ProjectionMatrix(Real screen_aspect, Real field_of_view, Real far_plane, Real near_plane) const
    {
        Real tan = tanf(field_of_view * 0.5f);

        Matrix44 result;
        result.data[0] = 1.0f / (screen_aspect * tan);
        result.data[5] = 1.0f / tan;
        result.data[10] = far_plane / (far_plane - near_plane);
        result.data[11] = 1.0f;
        result.data[14] = -near_plane * far_plane / (far_plane - near_plane);
        result.data[15] = 0.0f;

        return result;
    }
}

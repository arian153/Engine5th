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
        Real alpha = field_of_view * 0.5f;
        Real cot = cosf(alpha) / sinf(alpha);

        Matrix44 result;
        result.data[0] = cot / screen_aspect;
        result.data[5] = cot;
        result.data[10] = far_plane / (far_plane - near_plane);
        result.data[11] = 1.0f;
        result.data[14] = -near_plane * far_plane / (far_plane - near_plane);
        result.data[15] = 0.0f;

        return result;
    }
}

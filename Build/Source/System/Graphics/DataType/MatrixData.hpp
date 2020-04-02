#pragma once
#include "../../Math/Algebra/Matrix44.hpp"

namespace Engine5
{
    class MatrixData
    {
    public:
        MatrixData()
        {
        }

        MatrixData(const Matrix44& m, const Matrix44& v, const Matrix44& p)
            : model(m), view(v), projection(p)
        {
        }

        MatrixData(const Matrix44& v, const Matrix44& p)
            : view(v), projection(p)
        {
            model.SetIdentity();
        }

        ~MatrixData()
        {
        }

    public:
        Matrix44 model;
        Matrix44 view;
        Matrix44 projection;
    };
}

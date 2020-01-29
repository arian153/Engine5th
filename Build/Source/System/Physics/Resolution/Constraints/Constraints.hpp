#pragma once
#include "../../../Math/Math.hpp"

namespace Engine5
{
    class Constraints
    {
    public:
        Constraints();
        virtual ~Constraints();

        virtual void InitializeConstraints() = 0;
        virtual void SolveConstraints(Real dt) = 0;
        virtual void ApplyConstraints() = 0;

    private:
    };
}

#pragma once
#include "../../../Math/Math.hpp"

namespace Engine5
{
    class Constraint
    {
    public:
        Constraint();
        virtual ~Constraint();

        virtual void Initialize() = 0;
        virtual void SolveVelocityConstraints(Real dt) = 0;
        virtual void SolvePositionConstraints(Real dt) = 0;
        virtual void Apply() = 0;

    private:
    };
}

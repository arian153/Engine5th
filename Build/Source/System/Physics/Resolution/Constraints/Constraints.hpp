#pragma once
#include "../../../Math/Math.hpp"

namespace Engine5
{
    class Constraints
    {
    public:
        Constraints();
        virtual ~Constraints();

        virtual void Initialize() = 0;
        virtual void SolveVelocityConstraints(Real dt) = 0;
        virtual void SolvePositionConstraints(Real dt) = 0;
        virtual void Apply() = 0;

    private:
    };
}

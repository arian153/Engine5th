#pragma once
#include "Constraints.hpp"

namespace Engine5
{
    class PointConstraints : public Constraints
    {
    public:
        PointConstraints();
        ~PointConstraints();

        void Initialize() override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void Apply() override;
    private:
    };
}

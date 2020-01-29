#pragma once
#include "../../Math/Math.hpp"

namespace Engine5
{
    class Constraints;
    class ContactPoint;
    class ContactManifold;

    class Resolution
    {
    public:
        Resolution();
        ~Resolution();

        void SolveConstraints(Constraints* constraints, Real dt) const;

    private:
        bool m_b_warm_starting = true;
        Real baumgarte_scalar = 0.0035f;
        Real penetration_slop = 0.0005f;
        Real restitution_slop = 0.5f;

    };
}

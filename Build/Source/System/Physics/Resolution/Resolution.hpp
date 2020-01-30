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
    };
}

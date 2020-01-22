#pragma once
#include "../../Math/Math.hpp"

namespace Engine5
{
    class ContactPoint;
    class ContactManifold;

    class Resolution
    {
    public:
        Resolution();
        ~Resolution();

        void SolveConstraints() const;
        void SolveContact(ContactPoint* contact, Real dt) const;
        void SolveManifold(ContactManifold* manifold) const;
        void WarmStart(ContactManifold* manifold) const;

    private:
        bool m_b_warmstart = true;
        Real baumgarte_scalar = 0.0035f;
        Real penetration_slop = 0.0005f;
        Real restitution_slop = 0.5f;

    };
}

#pragma once
#include "../../Math/Math.hpp"

namespace Engine5
{
    class Contact;
    class Manifold;

    class Resolution
    {
    public:
        Resolution();
        ~Resolution();

        void SolveConstraints() const;
        void SolveContact(Contact* contact, Real dt) const;
        void SolveManifold() const;
        void WarmStart(Manifold* manifold) const;

    private:
        Real baumgarte_scalar = 0.0035f;
        Real penetration_slop = 0.0005f;
        Real restitution_slop = 0.5f;

    };
}

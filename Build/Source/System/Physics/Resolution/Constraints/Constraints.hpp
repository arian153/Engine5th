#pragma once

namespace Engine5
{
    class Constraints
    {
    public:
        Constraints();
        virtual ~Constraints();

        virtual void SolveConstraints(Real dt) = 0;
        virtual void ApplyConstraints() = 0;

    private:

    };
}

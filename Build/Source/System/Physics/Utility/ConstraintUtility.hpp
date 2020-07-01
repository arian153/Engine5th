#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    enum class eConstraintMode
    {
        Hard
      , Soft
    };

    class ConstraintCoefficient
    {
    public:
        Real damping = 0.0f;
        Real spring  = 0.0f;
    };

    class ConstraintDef
    {
    public:
        ConstraintDef();
        ~ConstraintDef();

        void SetConstraintMode(eConstraintMode mode);
        void SetFrequency(Real frequency);
        void SetDampingRatio(Real damping_ratio);

        ConstraintCoefficient GenerateCoefficient(Real mass) const;

    private:
        eConstraintMode m_mode          = eConstraintMode::Hard;
        Real            m_frequency     = 5.0f;
        Real            m_damping_ratio = 0.5f;
    };
}

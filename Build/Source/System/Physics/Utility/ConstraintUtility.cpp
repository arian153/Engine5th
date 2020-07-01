#include "ConstraintUtility.hpp"

namespace Engine5
{
    ConstraintUtility::ConstraintUtility()
    {
    }

    ConstraintUtility::~ConstraintUtility()
    {
    }

    void ConstraintUtility::SetConstraintMode(eConstraintMode mode)
    {
        m_mode = mode;
    }

    void ConstraintUtility::SetFrequency(Real frequency)
    {
        m_frequency = frequency;
    }

    void ConstraintUtility::SetDampingRatio(Real damping_ratio)
    {
        m_damping_ratio = damping_ratio;
    }

    ConstraintCoefficient ConstraintUtility::GenerateCoefficient(Real mass) const
    {
        Real angular_frequency = m_frequency * Math::TWO_PI;
        //result
        ConstraintCoefficient result;
        result.damping = 2.0f * mass * m_damping_ratio * angular_frequency;
        result.spring  = mass * angular_frequency * angular_frequency;
        return result;
    }
}

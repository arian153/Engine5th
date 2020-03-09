#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class FrameUtility
    {
    public:
        FrameUtility();
        ~FrameUtility();

        void CalculateFrameStatus(Real total_time);
        Real GetFramePerSecond() const;
        Real GetMillisecondPerFrame() const;
        Real GetSecondPerFrame() const;

    private:
        size_t m_frame_count  = 0;
        Real   m_time_elapsed = 0.0f;
        Real   m_curr_fps     = 0.0f;
        Real   m_curr_mspf    = 0.0f;
    };
}

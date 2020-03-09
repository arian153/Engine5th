#include "FrameUtility.hpp"

namespace Engine5
{
    FrameUtility::FrameUtility()
    {
    }

    FrameUtility::~FrameUtility()
    {
    }

    void FrameUtility::CalculateFrameStatus(Real total_time)
    {
        m_frame_count++;
        // Compute averages over one second period.
        if (total_time - m_time_elapsed >= 1.0f)
        {
            m_curr_fps  = (Real)m_frame_count; // fps = frame count / 1 second
            m_curr_mspf = 1000.0f / m_curr_fps;
            // Reset for next average.
            m_frame_count = 0;
            m_time_elapsed += 1.0f;
        }
    }
}

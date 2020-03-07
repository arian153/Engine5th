#include "TimeUtility.hpp"
#include <chrono>

namespace Engine5
{
    TimeUtility::TimeUtility()
        : m_tick_duration(), m_paused_duration()
    {
    }

    TimeUtility::~TimeUtility()
    {
    }

    Real TimeUtility::TotalTime() const
    {
        std::chrono::duration<R64> total_time = m_b_stopped ? m_stop_time - m_base_time : m_current_time - m_base_time;
        return static_cast<Real>(total_time.count() - m_paused_duration.count());
    }

    Real TimeUtility::DeltaTime() const
    {
        return static_cast<Real>(m_delta_time);
    }

    void TimeUtility::Reset()
    {
        m_current_time  = std::chrono::high_resolution_clock::now();
        m_base_time     = m_current_time;
        m_previous_time = m_current_time;
        m_stop_time     = std::chrono::time_point<std::chrono::high_resolution_clock>();
        m_b_stopped     = false;
    }

    void TimeUtility::Start()
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        if (m_b_stopped == true)
        {
            m_paused_duration += start_time - m_stop_time;
            m_previous_time = start_time;
            m_stop_time     = std::chrono::time_point<std::chrono::high_resolution_clock>();
            m_b_stopped     = false;
        }
    }

    void TimeUtility::Stop()
    {
        if (m_b_stopped == false)
        {
            m_stop_time = std::chrono::high_resolution_clock::now();
            m_b_stopped = true;
        }
    }

    void TimeUtility::Tick()
    {
        if (m_b_stopped == true)
        {
            m_delta_time = 0.0;
            return;
        }
        m_current_time  = std::chrono::high_resolution_clock::now();
        m_tick_duration = m_current_time - m_previous_time;
        m_delta_time    = m_tick_duration.count();
        m_delta_time    = m_delta_time > 0.0 ? m_delta_time : 0.0;
        m_previous_time = m_current_time;
    }
}

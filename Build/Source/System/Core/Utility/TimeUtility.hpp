#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "CoreDef.hpp"
#include <chrono>

namespace Engine5
{
    class TimeUtility
    {
    public:
        TimeUtility();
        ~TimeUtility();

        Real TotalTime() const;
        Real DeltaTime() const;

        void Reset();
        void Start();
        void Stop();
        void Tick();

    private:
        //time data
        std::chrono::time_point<std::chrono::high_resolution_clock> m_previous_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_current_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_base_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_paused_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_stop_time;

        //duration
        std::chrono::duration<R64> m_duration;

        //others
        R64  m_delta_time = -1.0;
        bool m_b_stopped  = false;
    };
}

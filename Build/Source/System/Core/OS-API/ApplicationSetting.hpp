#pragma once
#include "WindowMode.hpp"
#include "../../Math/Utility/MathDef.hpp"
#include <string>

namespace Engine5
{
    struct ApplicationSetting
    {
    public:
        bool b_confine_cursor;
        bool b_show_cursor;
        bool b_v_sync;

        int  screen_width;
        int  screen_height;
        Real screen_scale;

        std::string caption;
        eWindowMode window_mode;
    };
}

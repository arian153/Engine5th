#pragma once
#include "OSAPI.hpp"
#include IncludeGamePadAPI
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    struct AnalogStick
    {
        Real x;
        Real y;
    };

    const int MAXIMUM_GAME_PAD_BUTTON_COUNT = (int)KeyID_GamePad::GamePad_MAX;

    struct PadState
    {
        bool is_plugged;

        ButtonState state[ MAXIMUM_GPAD_BUTTON_COUNT ];


        Real trigger_R;
        Real trigger_L;

        Real thumb_R_X;
        Real thumb_R_Y;

        Real thumb_L_X;
        Real thumb_L_Y;
    };


    class InputGamePad : public InputGamePadAPI
    {
    public:
        InputGamePad();
        ~InputGamePad();

    private:
    };

    
}

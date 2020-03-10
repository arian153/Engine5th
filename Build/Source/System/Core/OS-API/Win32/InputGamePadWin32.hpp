#pragma once
#include <windows.h>
#include <Xinput.h>

namespace Engine5
{
    struct PadState;
    class InputGamePadWin32
    {
    public:
        InputGamePadWin32();
        ~InputGamePadWin32();

        void ProcessButtons(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;
        void ProcessThumb_L(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;
        void ProcessThumb_R(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;
        void ProcessTrigger(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;

    protected:
        const static size_t USER_MAX_COUNT = XUSER_MAX_COUNT;
        XINPUT_STATE        m_controllers[USER_MAX_COUNT];
    };
}

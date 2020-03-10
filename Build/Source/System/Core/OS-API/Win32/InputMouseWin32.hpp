#pragma once
#include <windows.h>
#include "../KeyCode.hpp"

namespace Engine5
{
    class InputMouseWin32
    {
    public:
        InputMouseWin32();
        ~InputMouseWin32();

        eKeyCodeMouse TranslateMessage(WPARAM wparam);
        int TranslateMessage(eKeyCodeMouse button_code);

    private:
    };
}

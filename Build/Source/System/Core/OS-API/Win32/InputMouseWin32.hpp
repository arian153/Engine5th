#pragma once
#include <windows.h>
#include "../Input/KeyCode.hpp"

namespace Engine5
{
    class InputMouseWin32
    {
    public:
        InputMouseWin32();
        ~InputMouseWin32();

    protected:
        eKeyCodeMouse TranslateMessage(WPARAM wparam);
        int           TranslateMessage(eKeyCodeMouse button_code);
        
    private:
        friend class OSWin32;
    };
}

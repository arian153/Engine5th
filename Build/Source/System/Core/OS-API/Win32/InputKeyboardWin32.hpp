#pragma once
#include <string>
#include <windows.h>

namespace Engine5
{
    enum class eKeyCodeKeyboard;

    class InputKeyboardWin32
    {
    public:
        InputKeyboardWin32();
        ~InputKeyboardWin32();

    protected:
        void             ProcessString(WPARAM wparam, std::wstring* string);
        eKeyCodeKeyboard TranslateMessage(WPARAM wparam);
        int              TranslateMessage(eKeyCodeKeyboard key_code);
    };
}

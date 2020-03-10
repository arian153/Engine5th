#include "InputKeyboardWin32.hpp"
#include "../InputKeyboard.hpp"
#include "../../Utility/CoreUtility.hpp"

namespace Engine5
{
    InputKeyboardWin32::InputKeyboardWin32()
    {
    }

    InputKeyboardWin32::~InputKeyboardWin32()
    {
    }

    void InputKeyboardWin32::ProcessString(WPARAM wparam, std::wstring* string)
    {
        if (string != nullptr)
        {
            string->push_back((wchar_t)wparam);
        }
    }

    eKeyCodeKeyboard InputKeyboardWin32::TranslateMessage(WPARAM wparam)
    {
        switch (wparam)
        {
        case VK_RETURN:
            return eKeyCodeKeyboard::Keyboard_Enter;
        case VK_ESCAPE:
            return eKeyCodeKeyboard::Keyboard_Escape;
        case VK_BACK:
            return eKeyCodeKeyboard::Keyboard_Backspace;
        case VK_TAB:
            return eKeyCodeKeyboard::Keyboard_Tab;
        case VK_SPACE:
            return eKeyCodeKeyboard::Keyboard_Space;
        case VK_OEM_COMMA:
            return eKeyCodeKeyboard::Keyboard_Comma;
        case VK_OEM_MINUS:
            return eKeyCodeKeyboard::Keyboard_Minus;
        case VK_OEM_PERIOD:
            return eKeyCodeKeyboard::Keyboard_Period;
        case 0x30:
            return eKeyCodeKeyboard::Keyboard_0;
        case 0x31:
            return eKeyCodeKeyboard::Keyboard_1;
        case 0x32:
            return eKeyCodeKeyboard::Keyboard_2;
        case 0x33:
            return eKeyCodeKeyboard::Keyboard_3;
        case 0x34:
            return eKeyCodeKeyboard::Keyboard_4;
        case 0x35:
            return eKeyCodeKeyboard::Keyboard_5;
        case 0x36:
            return eKeyCodeKeyboard::Keyboard_6;
        case 0x37:
            return eKeyCodeKeyboard::Keyboard_7;
        case 0x38:
            return eKeyCodeKeyboard::Keyboard_8;
        case 0x39:
            return eKeyCodeKeyboard::Keyboard_9;
        case VK_CAPITAL:
            return eKeyCodeKeyboard::Keyboard_CapsLock;
        case 0x41:
            return eKeyCodeKeyboard::Keyboard_A;
        case 0x42:
            return eKeyCodeKeyboard::Keyboard_B;
        case 0x43:
            return eKeyCodeKeyboard::Keyboard_C;
        case 0x44:
            return eKeyCodeKeyboard::Keyboard_D;
        case 0x45:
            return eKeyCodeKeyboard::Keyboard_E;
        case 0x46:
            return eKeyCodeKeyboard::Keyboard_F;
        case 0x47:
            return eKeyCodeKeyboard::Keyboard_G;
        case 0x48:
            return eKeyCodeKeyboard::Keyboard_H;
        case 0x49:
            return eKeyCodeKeyboard::Keyboard_I;
        case 0x4A:
            return eKeyCodeKeyboard::Keyboard_J;
        case 0x4B:
            return eKeyCodeKeyboard::Keyboard_K;
        case 0x4C:
            return eKeyCodeKeyboard::Keyboard_L;
        case 0x4D:
            return eKeyCodeKeyboard::Keyboard_M;
        case 0x4E:
            return eKeyCodeKeyboard::Keyboard_N;
        case 0x4F:
            return eKeyCodeKeyboard::Keyboard_O;
        case 0x50:
            return eKeyCodeKeyboard::Keyboard_P;
        case 0x51:
            return eKeyCodeKeyboard::Keyboard_Q;
        case 0x52:
            return eKeyCodeKeyboard::Keyboard_R;
        case 0x53:
            return eKeyCodeKeyboard::Keyboard_S;
        case 0x54:
            return eKeyCodeKeyboard::Keyboard_T;
        case 0x55:
            return eKeyCodeKeyboard::Keyboard_U;
        case 0x56:
            return eKeyCodeKeyboard::Keyboard_V;
        case 0x57:
            return eKeyCodeKeyboard::Keyboard_W;
        case 0x58:
            return eKeyCodeKeyboard::Keyboard_X;
        case 0x59:
            return eKeyCodeKeyboard::Keyboard_Y;
        case 0x5A:
            return eKeyCodeKeyboard::Keyboard_Z;
        case VK_F1:
            return eKeyCodeKeyboard::Keyboard_F1;
        case VK_F2:
            return eKeyCodeKeyboard::Keyboard_F2;
        case VK_F3:
            return eKeyCodeKeyboard::Keyboard_F3;
        case VK_F4:
            return eKeyCodeKeyboard::Keyboard_F4;
        case VK_F5:
            return eKeyCodeKeyboard::Keyboard_F5;
        case VK_F6:
            return eKeyCodeKeyboard::Keyboard_F6;
        case VK_F7:
            return eKeyCodeKeyboard::Keyboard_F7;
        case VK_F8:
            return eKeyCodeKeyboard::Keyboard_F8;
        case VK_F9:
            return eKeyCodeKeyboard::Keyboard_F9;
        case VK_F10:
            return eKeyCodeKeyboard::Keyboard_F10;
        case VK_F11:
            return eKeyCodeKeyboard::Keyboard_F11;
        case VK_F12:
            return eKeyCodeKeyboard::Keyboard_F12;
        case VK_SNAPSHOT:
            return eKeyCodeKeyboard::Keyboard_PrintScreen;
        case VK_SCROLL:
            return eKeyCodeKeyboard::Keyboard_ScrollLock;
        case VK_PAUSE:
            return eKeyCodeKeyboard::Keyboard_Pause;
        case VK_INSERT:
            return eKeyCodeKeyboard::Keyboard_Insert;
        case VK_HOME:
            return eKeyCodeKeyboard::Keyboard_Home;
        case VK_PRIOR:
            return eKeyCodeKeyboard::Keyboard_PageUp;
        case VK_DELETE:
            return eKeyCodeKeyboard::Keyboard_Delete;
        case VK_END:
            return eKeyCodeKeyboard::Keyboard_End;
        case VK_NEXT:
            return eKeyCodeKeyboard::Keyboard_PageDown;
        case VK_RIGHT:
            return eKeyCodeKeyboard::Keyboard_Arrow_Right;
        case VK_LEFT:
            return eKeyCodeKeyboard::Keyboard_Arrow_Left;
        case VK_DOWN:
            return eKeyCodeKeyboard::Keyboard_Arrow_Down;
        case VK_UP:
            return eKeyCodeKeyboard::Keyboard_Arrow_Up;
        case VK_APPS:
            return eKeyCodeKeyboard::Keyboard_Application;
        case VK_LMENU:
            return eKeyCodeKeyboard::Keyboard_Alt_Left;
        case VK_RMENU:
            return eKeyCodeKeyboard::Keyboard_Alt_Right;
        case VK_SELECT:
            return eKeyCodeKeyboard::Keyboard_Select;
        case VK_LCONTROL:
            return eKeyCodeKeyboard::Keyboard_Control_Left;
        case VK_LSHIFT:
            return eKeyCodeKeyboard::Keyboard_Shift_Left;
        case VK_RCONTROL:
            return eKeyCodeKeyboard::Keyboard_Control_Right;
        case VK_RSHIFT:
            return eKeyCodeKeyboard::Keyboard_Shift_Right;
        default:
            return eKeyCodeKeyboard::Keyboard_NONE;
        }
    }

    int InputKeyboardWin32::TranslateMessage(eKeyCodeKeyboard key_code)
    {
        switch (key_code)
        {
        case eKeyCodeKeyboard::Keyboard_NONE:
            return 0;
        case eKeyCodeKeyboard::Keyboard_0:
            return 0x30;
        case eKeyCodeKeyboard::Keyboard_1:
            return 0x31;
        case eKeyCodeKeyboard::Keyboard_2:
            return 0x32;
        case eKeyCodeKeyboard::Keyboard_3:
            return 0x33;
        case eKeyCodeKeyboard::Keyboard_4:
            return 0x34;
        case eKeyCodeKeyboard::Keyboard_5:
            return 0x35;
        case eKeyCodeKeyboard::Keyboard_6:
            return 0x36;
        case eKeyCodeKeyboard::Keyboard_7:
            return 0x37;
        case eKeyCodeKeyboard::Keyboard_8:
            return 0x38;
        case eKeyCodeKeyboard::Keyboard_9:
            return 0x39;
        case eKeyCodeKeyboard::Keyboard_A:
            return 0x41;
        case eKeyCodeKeyboard::Keyboard_B:
            return 0x42;
        case eKeyCodeKeyboard::Keyboard_C:
            return 0x43;
        case eKeyCodeKeyboard::Keyboard_D:
            return 0x44;
        case eKeyCodeKeyboard::Keyboard_E:
            return 0x45;
        case eKeyCodeKeyboard::Keyboard_F:
            return 0x46;
        case eKeyCodeKeyboard::Keyboard_G:
            return 0x47;
        case eKeyCodeKeyboard::Keyboard_H:
            return 0x48;
        case eKeyCodeKeyboard::Keyboard_I:
            return 0x49;
        case eKeyCodeKeyboard::Keyboard_J:
            return 0x4A;
        case eKeyCodeKeyboard::Keyboard_K:
            return 0x4B;
        case eKeyCodeKeyboard::Keyboard_L:
            return 0x4C;
        case eKeyCodeKeyboard::Keyboard_M:
            return 0x4D;
        case eKeyCodeKeyboard::Keyboard_N:
            return 0x4E;
        case eKeyCodeKeyboard::Keyboard_O:
            return 0x4F;
        case eKeyCodeKeyboard::Keyboard_P:
            return 0x50;
        case eKeyCodeKeyboard::Keyboard_Q:
            return 0x51;
        case eKeyCodeKeyboard::Keyboard_R:
            return 0x52;
        case eKeyCodeKeyboard::Keyboard_S:
            return 0x53;
        case eKeyCodeKeyboard::Keyboard_T:
            return 0x54;
        case eKeyCodeKeyboard::Keyboard_U:
            return 0x55;
        case eKeyCodeKeyboard::Keyboard_V:
            return 0x56;
        case eKeyCodeKeyboard::Keyboard_W:
            return 0x57;
        case eKeyCodeKeyboard::Keyboard_X:
            return 0x58;
        case eKeyCodeKeyboard::Keyboard_Y:
            return 0x59;
        case eKeyCodeKeyboard::Keyboard_Z:
            return 0x5A;
        case eKeyCodeKeyboard::Keyboard_F1:
            return VK_F1;
        case eKeyCodeKeyboard::Keyboard_F2:
            return VK_F2;
        case eKeyCodeKeyboard::Keyboard_F3:
            return VK_F3;
        case eKeyCodeKeyboard::Keyboard_F4:
            return VK_F4;
        case eKeyCodeKeyboard::Keyboard_F5:
            return VK_F5;
        case eKeyCodeKeyboard::Keyboard_F6:
            return VK_F6;
        case eKeyCodeKeyboard::Keyboard_F7:
            return VK_F7;
        case eKeyCodeKeyboard::Keyboard_F8:
            return VK_F8;
        case eKeyCodeKeyboard::Keyboard_F9:
            return VK_F9;
        case eKeyCodeKeyboard::Keyboard_F10:
            return VK_F10;
        case eKeyCodeKeyboard::Keyboard_F11:
            return VK_F11;
        case eKeyCodeKeyboard::Keyboard_F12:
            return VK_F12;
        case eKeyCodeKeyboard::Keyboard_Arrow_Up:
            return VK_UP;
        case eKeyCodeKeyboard::Keyboard_Arrow_Down:
            return VK_DOWN;
        case eKeyCodeKeyboard::Keyboard_Arrow_Right:
            return VK_RIGHT;
        case eKeyCodeKeyboard::Keyboard_Arrow_Left:
            return VK_LEFT;
        case eKeyCodeKeyboard::Keyboard_Enter:
            return VK_RETURN;
        case eKeyCodeKeyboard::Keyboard_Escape:
            return VK_ESCAPE;
        case eKeyCodeKeyboard::Keyboard_Backspace:
            return VK_BACK;
        case eKeyCodeKeyboard::Keyboard_Tab:
            return VK_TAB;
        case eKeyCodeKeyboard::Keyboard_Space:
            return VK_SPACE;
        case eKeyCodeKeyboard::Keyboard_Control_Left:
            return VK_LCONTROL;
        case eKeyCodeKeyboard::Keyboard_Control_Right:
            return VK_RCONTROL;
        case eKeyCodeKeyboard::Keyboard_Shift_Left:
            return VK_LSHIFT;
        case eKeyCodeKeyboard::Keyboard_Shift_Right:
            return VK_RSHIFT;
        case eKeyCodeKeyboard::Keyboard_Alt_Left:
            return VK_LMENU;
        case eKeyCodeKeyboard::Keyboard_Alt_Right:
            return VK_RMENU;
        case eKeyCodeKeyboard::Keyboard_Quote:
            return 0;
        case eKeyCodeKeyboard::Keyboard_Comma:
            return VK_OEM_COMMA;
        case eKeyCodeKeyboard::Keyboard_Minus:
            return VK_OEM_MINUS;
        case eKeyCodeKeyboard::Keyboard_Period:
            return VK_OEM_PERIOD;
        case eKeyCodeKeyboard::Keyboard_Slash:
            return 0;
        case eKeyCodeKeyboard::Keyboard_Semicolon:
            return 0;
        case eKeyCodeKeyboard::Keyboard_Equal:
            return 0;
        case eKeyCodeKeyboard::Keyboard_Bracket_Left:
            return 0;
        case eKeyCodeKeyboard::Keyboard_Bracket_Right:
            return 0;
        case eKeyCodeKeyboard::Keyboard_Backslash:
            return 0;
        case eKeyCodeKeyboard::Keyboard_PrintScreen:
            return VK_SNAPSHOT;
        case eKeyCodeKeyboard::Keyboard_ScrollLock:
            return VK_SCROLL;
        case eKeyCodeKeyboard::Keyboard_Pause:
            return VK_PAUSE;
        case eKeyCodeKeyboard::Keyboard_CapsLock:
            return VK_CAPITAL;
        case eKeyCodeKeyboard::Keyboard_Insert:
            return VK_INSERT;
        case eKeyCodeKeyboard::Keyboard_Home:
            return VK_HOME;
        case eKeyCodeKeyboard::Keyboard_PageUp:
            return VK_PRIOR;
        case eKeyCodeKeyboard::Keyboard_Delete:
            return VK_DELETE;
        case eKeyCodeKeyboard::Keyboard_End:
            return VK_END;
        case eKeyCodeKeyboard::Keyboard_PageDown:
            return VK_NEXT;
        case eKeyCodeKeyboard::Keyboard_Application:
            return VK_APPS;
        case eKeyCodeKeyboard::Keyboard_Power:
            return 0;
        case eKeyCodeKeyboard::Keyboard_Select:
            return VK_SELECT;
        case eKeyCodeKeyboard::Keyboard_MAX:
            return 0;
        default:
            return 0;
        }
    }

    InputKeyboard::InputKeyboard()
    {
    }

    InputKeyboard::~InputKeyboard()
    {
    }

    void InputKeyboard::Reset()
    {
        for (size_t i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            m_keyboard[i].b_down         = false;
            m_keyboard[i].b_curr_pressed = false;
            m_keyboard[i].b_prev_pressed = false;
        }
    }

    void InputKeyboard::ResetPressed()
    {
        for (size_t i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            m_keyboard[i].b_curr_pressed = false;
        }
    }

    void InputKeyboard::ProcessPressed()
    {
        for (int i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            if (GetAsyncKeyState(TranslateMessage(static_cast<eKeyCodeKeyboard>(i))) & 0x8000)
            {
                m_keyboard[i].b_down = true;
                /* KeyboardEvent key_down_event(ButtonEventState::ButtonDown);
                 key_down_event.key = static_cast<KeyID_Keyboard>(i);
                 m_subject->Notify(key_down_event, KeyboardEvent::StateToString(ButtonEventState::ButtonDown));*/
            }
            else
            {
                /*if (m_keyboard[ i ].b_curr_pressed== false && m_keyboard[ i ].b_prev_pressed == true)
                {
                    KeyboardEvent key_up_event(ButtonEventState::ButtonUp);
                    key_up_event.key = static_cast<KeyID_Keyboard>(i);
                    m_subject->Notify(key_up_event, KeyboardEvent::StateToString(ButtonEventState::ButtonUp));
                }*/
                m_keyboard[i].b_down         = false;
                m_keyboard[i].b_prev_pressed = false;
                m_keyboard[i].b_curr_pressed = false;
            }
            if (m_keyboard[i].b_down == true)
            {
                if (m_keyboard[i].b_curr_pressed == false && m_keyboard[i].b_prev_pressed == false)
                {
                    m_keyboard[i].b_curr_pressed = true;
                    m_keyboard[i].b_prev_pressed = false;
                    /* KeyboardEvent key_press_event(ButtonEventState::ButtonPressed);
                     key_press_event.key = static_cast<KeyID_Keyboard>(i);
                     m_subject->Notify(key_press_event, KeyboardEvent::StateToString(ButtonEventState::ButtonPressed));*/
                }
                else if (m_keyboard[i].b_curr_pressed == true && m_keyboard[i].b_prev_pressed == false)
                {
                    m_keyboard[i].b_curr_pressed = false;
                    m_keyboard[i].b_prev_pressed = true;
                }
            }
        }
    }

    bool InputKeyboard::IsDown(eKeyCodeKeyboard key_code) const
    {
        return m_keyboard[static_cast<int>(key_code)].b_down;
    }

    bool InputKeyboard::IsPressed(eKeyCodeKeyboard key_code) const
    {
        return m_keyboard[static_cast<int>(key_code)].b_curr_pressed;
    }

    bool InputKeyboard::IsAnyKeyDown() const
    {
        for (size_t i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            if (m_keyboard[i].b_down == true)
                return true;
        }
        return false;
    }

    bool InputKeyboard::IsAnyKeyPressed() const
    {
        for (int i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            if (m_keyboard[i].b_curr_pressed == true)
                return true;
        }
        return false;
    }

    size_t InputKeyboard::Repeated(eKeyCodeKeyboard key_code) const
    {
        E5_UNUSED_PARAM(key_code);
        return 0;
    }

    std::string InputKeyboard::TypedString() const
    {
        return WStringToString(*m_edited_string);
    }

    std::wstring* InputKeyboard::TypedWString() const
    {
        return m_edited_string;
    }

    void InputKeyboard::SetEditingWString(std::wstring* string)
    {
        m_edited_string = string;
    }
}

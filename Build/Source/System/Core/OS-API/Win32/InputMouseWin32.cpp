#include "InputMouseWin32.hpp"
#include "../InputMouse.hpp"
#include "../../Utility/CoreUtility.hpp"

namespace Engine5
{
    InputMouseWin32::InputMouseWin32()
    {
    }

    InputMouseWin32::~InputMouseWin32()
    {
    }

    eKeyCodeMouse InputMouseWin32::TranslateMessage(WPARAM wparam)
    {
        switch (wparam)
        {
        case VK_LBUTTON:
            return eKeyCodeMouse::Mouse_Left;
        case VK_RBUTTON:
            return eKeyCodeMouse::Mouse_Right;
        case VK_MBUTTON:
            return eKeyCodeMouse::Mouse_Middle;
        case VK_XBUTTON1:
            return eKeyCodeMouse::Mouse_X1;
        case VK_XBUTTON2:
            return eKeyCodeMouse::Mouse_X2;
        default:
            return eKeyCodeMouse::Mouse_NONE;
        }
    }

    int InputMouseWin32::TranslateMessage(eKeyCodeMouse button_code)
    {
        switch (button_code)
        {
        case eKeyCodeMouse::Mouse_NONE:
            return 0;
        case eKeyCodeMouse::Mouse_Left:
            return VK_LBUTTON;
        case eKeyCodeMouse::Mouse_Right:
            return VK_RBUTTON;
        case eKeyCodeMouse::Mouse_Middle:
            return VK_MBUTTON;
        case eKeyCodeMouse::Mouse_X1:
            return VK_XBUTTON1;
        case eKeyCodeMouse::Mouse_X2:
            return VK_XBUTTON2;
        case eKeyCodeMouse::Mouse_Max:
            return 0;
        default:
            return 0;
        }
    }

    InputMouse::InputMouse()
        : m_current_position(), m_previous_position(), m_button_state{}
    {
    }

    InputMouse::~InputMouse()
    {
    }

    void InputMouse::Reset()
    {
        for (size_t i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            m_button_state[i].b_down         = false;
            m_button_state[i].b_curr_pressed = false;
            m_button_state[i].b_prev_pressed = false;
        }
        m_current_wheel    = 0;
        m_previous_wheel   = 0;
        m_b_was_wheel      = false;
        m_b_was_mouse_move = false;
    }

    void InputMouse::ResetPressed()
    {
        for (size_t i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            m_button_state[i].b_curr_pressed = false;
        }
    }

    void InputMouse::ProcessMouseEvent(bool down, eKeyCodeMouse code, int x, int y)
    {
        m_button_state[static_cast<size_t>(code)].b_down = down;
        /*if (down == false)
        {
            MouseEvent ms_event(ButtonEventState::ButtonUp);
            ms_event.key = code;
            m_subject->Notify(ms_event, MouseEvent::StateToString(ButtonEventState::ButtonUp));
        }*/
        m_previous_position.x = m_current_position.x;
        m_previous_position.y = m_current_position.y;
        m_current_position.x  = x;
        m_current_position.y  = y;
        m_b_was_mouse_move    = true;
    }

    void InputMouse::ProcessMouseEvent(int x, int y)
    {
        m_previous_position.x = m_current_position.x;
        m_previous_position.y = m_current_position.y;
        m_current_position.x  = x;
        m_current_position.y  = y;
        m_b_was_mouse_move    = true;
    }

    void InputMouse::ProcessMouseWheel(short wheel)
    {
        m_previous_wheel = m_current_wheel;
        m_current_wheel += wheel;
        m_b_was_wheel = true;
    }

    void InputMouse::ProcessPressed()
    {
        for (int i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            if (m_button_state[i].b_down == true)
            {
                //MouseEvent down_event(ButtonEventState::ButtonDown);
                //down_event.key = static_cast<KeyID_Mouse>(i);
                //m_subject->Notify(down_event, MouseEvent::StateToString(ButtonEventState::ButtonDown));
                if (m_button_state[i].b_curr_pressed == false && m_button_state[i].b_prev_pressed == false)
                {
                    m_button_state[i].b_curr_pressed = true;
                    m_button_state[i].b_prev_pressed = false;
                    //MouseEvent press_event(ButtonEventState::ButtonPressed);
                    //press_event.key = static_cast<KeyID_Mouse>(i);
                    //m_subject->Notify(press_event, MouseEvent::StateToString(ButtonEventState::ButtonPressed));
                }
                else if (m_button_state[i].b_curr_pressed == true && m_button_state[i].b_prev_pressed == false)
                {
                    m_button_state[i].b_curr_pressed = false;
                    m_button_state[i].b_prev_pressed = true;
                }
            }
            else
            {
                m_button_state[i].b_curr_pressed = false;
                m_button_state[i].b_prev_pressed = false;
            }
        }
        if (m_b_was_wheel == true)
        {
            //use goto and skip below code.
            m_b_was_wheel = false;
            goto mouse_move;
        }
        if (m_b_was_wheel == false && m_current_wheel != m_previous_wheel)
        {
            m_previous_wheel = m_current_wheel;
        }
    mouse_move:
        if (m_b_was_mouse_move == true)
        {
            m_b_was_mouse_move = false;
            return;
        }
        if (m_b_was_mouse_move == false && (m_current_position.x != m_previous_position.x || m_current_position.y != m_previous_position.y))
        {
            m_previous_position.x = m_current_position.x;
            m_previous_position.y = m_current_position.y;
        }
    }

    bool InputMouse::IsDown(eKeyCodeMouse key_code) const
    {
        return m_button_state[static_cast<size_t>(key_code)].b_down;
    }

    bool InputMouse::IsPressed(eKeyCodeMouse key_code) const
    {
        return m_button_state[static_cast<size_t>(key_code)].b_curr_pressed;
    }

    bool InputMouse::IsAnyKeyDown() const
    {
        for (size_t i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            if (m_button_state[i].b_down == true)
                return true;
        }
        return false;
    }

    bool InputMouse::IsAnyKeyPressed() const
    {
        for (int i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            if (m_button_state[i].b_curr_pressed == true)
                return true;
        }
        return false;
    }

    size_t InputMouse::Repeated(eKeyCodeMouse key_code) const
    {
        E5_UNUSED_PARAM(key_code);
        return 0;
    }

    MousePosition InputMouse::CurrentPosition() const
    {
        return m_current_position;
    }

    MousePosition InputMouse::PreviousPosition() const
    {
        return m_previous_position;
    }

    int InputMouse::CurrentMouseWheel() const
    {
        return static_cast<int>(m_current_wheel);
    }

    int InputMouse::PreviousMouseWheel() const
    {
        return static_cast<int>(m_previous_wheel);
    }

    bool InputMouse::IsWheelRolling() const
    {
        return m_current_wheel != m_previous_wheel;
    }

    Real InputMouse::MouseWheelRollingDirection() const
    {
        int wheel_value = m_current_wheel - m_previous_wheel;
        return static_cast<Real>(wheel_value) / fabsf(static_cast<Real>(wheel_value));
    }
}

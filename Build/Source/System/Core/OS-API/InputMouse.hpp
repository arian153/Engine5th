#pragma once
#include "OSAPI.hpp"
#include IncludeMouseAPI
#include "KeyCode.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    struct MousePosition
    {
        int x;
        int y;
    };

    class InputMouse : public InputMouseAPI
    {
    public:
        InputMouse();
        ~InputMouse();

    private:
        const static size_t MAXIMUM_BUTTON_COUNT = (size_t)eKeyCodeMouse::Mouse_Max;

        void Reset();
        void ResetPressed();

        void ProcessMouseEvent(bool down, eKeyCodeMouse code, int x, int y);
        void ProcessMouseEvent(int x, int y);
        void ProcessMouseWheel(short wheel);
        void ProcessPressed();

    public: //interface 
        bool IsDown(eKeyCodeMouse key_code) const;
        bool IsPressed(eKeyCodeMouse key_code) const;

        bool IsAnyKeyDown() const;
        bool IsAnyKeyPressed() const;

        size_t Repeated(eKeyCodeMouse key_code) const;

        MousePosition CurrentPosition() const;
        MousePosition PreviousPosition() const;

        int  CurrentMouseWheel() const;
        int  PreviousMouseWheel() const;
        bool IsWheelRolling() const;
        Real MouseWheelRollingDirection() const;

    private:
        MousePosition m_current_position;
        MousePosition m_previous_position;
        ButtonState   m_button_state[ MAXIMUM_BUTTON_COUNT ];
        short         m_current_wheel    = 0;
        short         m_previous_wheel   = 0;
        bool          m_b_was_wheel      = false;
        bool          m_b_was_mouse_move = false;
    };
}

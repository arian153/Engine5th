#pragma once

namespace Engine5
{
    enum class eKeyCodeMouse
    {
        Mouse_NONE
      , Mouse_Left
      , Mouse_Right
      , Mouse_Middle
      , Mouse_X1
      , Mouse_X2
      , Mouse_Max
    };

    enum class eKeyCodeKeyboard
    {
        Keyboard_NONE
      , Keyboard_0
      , Keyboard_1
      , Keyboard_2
      , Keyboard_3
      , Keyboard_4
      , Keyboard_5
      , Keyboard_6
      , Keyboard_7
      , Keyboard_8
      , Keyboard_9
      , Keyboard_A
      , Keyboard_B
      , Keyboard_C
      , Keyboard_D
      , Keyboard_E
      , Keyboard_F
      , Keyboard_G
      , Keyboard_H
      , Keyboard_I
      , Keyboard_J
      , Keyboard_K
      , Keyboard_L
      , Keyboard_M
      , Keyboard_N
      , Keyboard_O
      , Keyboard_P
      , Keyboard_Q
      , Keyboard_R
      , Keyboard_S
      , Keyboard_T
      , Keyboard_U
      , Keyboard_V
      , Keyboard_W
      , Keyboard_X
      , Keyboard_Y
      , Keyboard_Z
      , Keyboard_F1
      , Keyboard_F2
      , Keyboard_F3
      , Keyboard_F4
      , Keyboard_F5
      , Keyboard_F6
      , Keyboard_F7
      , Keyboard_F8
      , Keyboard_F9
      , Keyboard_F10
      , Keyboard_F11
      , Keyboard_F12
      , Keyboard_Arrow_Up
      , Keyboard_Arrow_Down
      , Keyboard_Arrow_Right
      , Keyboard_Arrow_Left
      , Keyboard_Enter
      , Keyboard_Escape
      , Keyboard_Backspace
      , Keyboard_Tab
      , Keyboard_Space
      , Keyboard_Control_Left
      , Keyboard_Control_Right
      , Keyboard_Shift_Left
      , Keyboard_Shift_Right
      , Keyboard_Alt_Left
      , Keyboard_Alt_Right
      , Keyboard_Period
      , Keyboard_Comma
      , Keyboard_Slash
      , Keyboard_Semicolon
      , Keyboard_Quote
      , Keyboard_Bracket_Left
      , Keyboard_Bracket_Right
      , Keyboard_Backslash
      , Keyboard_Minus
      , Keyboard_Equal
      , Keyboard_PrintScreen
      , Keyboard_ScrollLock
      , Keyboard_Pause
      , Keyboard_CapsLock
      , Keyboard_Insert
      , Keyboard_Home
      , Keyboard_PageUp
      , Keyboard_Delete
      , Keyboard_End
      , Keyboard_PageDown
      , Keyboard_Application
      , Keyboard_Power
      , Keyboard_Select
      , Keyboard_MAX
    };

    enum class eKeyCodeGamePad
    {
        GamePad_NONE
      , GamePad_DPAD_Up
      , GamePad_DPAD_Down
      , GamePad_DPAD_Right
      , GamePad_DPAD_Left
      , GamePad_Start
      , GamePad_Back
      , GamePad_X
      , GamePad_Y
      , GamePad_A
      , GamePad_B
      , GamePad_Shoulder_Left
      , GamePad_Shoulder_Right
      , GamePad_Thumb_Left
      , GamePad_Thumb_Right
      , GamePad_MAX
    };

    enum class eKeyCodeTextEditor
    {
        TextEdit_Tab
      , TextEdit_LeftArrow
      , TextEdit_RightArrow
      , TextEdit_UpArrow
      , TextEdit_DownArrow
      , TextEdit_PageUp
      , TextEdit_PageDown
      , TextEdit_Home
      , TextEdit_End
      , TextEdit_Insert
      , TextEdit_Delete
      , TextEdit_Backspace
      , TextEdit_Space
      , TextEdit_Enter
      , TextEdit_Esc
      , TextEdit_A //CTRL+A: select all
      , TextEdit_C //CTRL+C: copy
      , TextEdit_V //CTRL+V: paste
      , TextEdit_X //CTRL+X: cut
      , TextEdit_Y //CTRL+Y: redo
      , TextEdit_Z //CTRL+Z: undo
      , TextEdit_MAX
    };

    enum class eKeyCodeGeneralEditor
    {
        Editor_Delete
      , Editor_C //CTRL+C: copy
      , Editor_V //CTRL+V: paste
      , Editor_X //CTRL+X: cut
      , Editor_Y //CTRL+Y: redo
      , Editor_Z //CTRL+Z: undo
      , Editor_MAX
    };

    struct ButtonState
    {
        bool b_prev_pressed = false;
        bool b_curr_pressed = false;
        bool b_down         = false;
    };

    enum class ButtonEventState
    {
        ButtonUp
      , ButtonDown
      , ButtonPressed
    };
}

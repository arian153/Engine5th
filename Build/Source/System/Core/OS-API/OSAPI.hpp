#pragma once
#define Win32

#if defined(Win32)
//classes
#define OSAPI OSWin32
#define InputKeyboardAPI InputKeyboardWin32
#define InputMouseAPI InputMouseWin32
#define InputGamePadAPI InputGamePadWin32
//includes
#define IncludeOSAPI "Win32/OSWin32.hpp"
#define IncludeKeyboardAPI "Win32/InputKeyboardWin32.hpp"
#define IncludeMouseAPI "Win32/InputMouseWin32.hpp"
#define IncludeGamePadAPI "Win32/InputGamePadWin32.hpp"

#elif defined(UWP)
//classes
#define OSAPI OSUWP

//includes
#define IncludeOSAPI "UWP/OSUWP.hpp"

#endif

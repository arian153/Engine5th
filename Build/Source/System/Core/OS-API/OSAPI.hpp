#pragma once
#define Win32

#if defined(Win32)
//classes
#define OSAPI OSWin32

//includes
#define IncludeOSAPI "Win32/OSWin32.hpp"

#elif defined(UWP)
//classes
#define OSAPI OSUWP

//includes
#define IncludeOSAPI "UWP/OSUWP.hpp"

#endif

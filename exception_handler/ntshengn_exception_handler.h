#pragma once
#if defined(NTSHENGN_OS_WINDOWS)
#include "ntshengn_exception_handler_windows.h"
#elif defined(NTSHENGN_OS_LINUX) || defined(NTSHENGN_OS_FREEBSD)
#include "ntshengn_exception_handler_linux_freebsd.h"
#endif
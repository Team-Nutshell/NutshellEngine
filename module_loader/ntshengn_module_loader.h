#pragma once
#if defined(NTSHENGN_OS_WINDOWS)
#include "ntshengn_module_loader_windows.h"
#elif defined(NTSHENGN_OS_LINUX) || defined(NTSHENGN_OS_FREEBSD)
#include "ntshengn_module_loader_linux_freebsd.h"
#endif
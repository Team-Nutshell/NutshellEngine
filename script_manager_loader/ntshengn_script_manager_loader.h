#pragma once
#if defined(NTSHENGN_OS_WINDOWS)
#include "ntshengn_script_manager_loader_windows.h"
#elif defined(NTSHENGN_OS_LINUX) || defined(NTSHENGN_OS_FREEBSD)
#include "ntshengn_script_manager_loader_linux_freebsd.h"
#endif
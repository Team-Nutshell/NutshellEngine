#pragma once
#if defined(NTSHENGN_OS_WINDOWS)
#include "ntshengn_module_loader_windows.h"
#elif defined(NTSHENGN_OS_LINUX)
#include "ntshengn_module_loader_linux.h"
#endif
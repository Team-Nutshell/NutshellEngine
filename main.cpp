#if defined(NTSHENGN_OS_WINDOWS) && defined(NTSHENGN_RELEASE)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "ntshengn_core.h"
#if defined(NTSHENGN_DEBUG)
#include "exception_handler/ntshengn_exception_handler.h"
#endif

int main() {
#if defined(NTSHENGN_DEBUG)
	NtshEngn::ExceptionHandler exceptionHandler;
	exceptionHandler.setExceptionHandler();
#endif

	NtshEngn::Core core;
	core.run("assets/options/options.ntop");

#if defined(NTSHENGN_DEBUG)
	exceptionHandler.unsetExceptionHandler();
#endif

	return 0;
}
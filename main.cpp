#if defined(NTSHENGN_OS_WINDOWS) && defined(NTSHENGN_RELEASE)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "ntshengn_core.h"
#if defined(NTSHENGN_DEBUG)
#include "exception_handler/ntshengn_exception_handler.h"
#endif

int main(int argc, char** argv) {
#if defined(NTSHENGN_DEBUG)
	NtshEngn::ExceptionHandler exceptionHandler;
	exceptionHandler.setExceptionHandler();
#endif

	NtshEngn::CommandLine commandLine;
	commandLine.set(argc, argv);

	NtshEngn::Core core;
	core.run("assets/options/options.ntop", commandLine);

#if defined(NTSHENGN_DEBUG)
	exceptionHandler.unsetExceptionHandler();
#endif

	return 0;
}
#if defined(NTSHENGN_OS_WINDOWS) && defined(NTSHENGN_RELEASE)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "ntshengn_core.h"

int main() {
	NtshEngn::Core core;
	core.run("assets/options/options.ntop");

	return 0;
}
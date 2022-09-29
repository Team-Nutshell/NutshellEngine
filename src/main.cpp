#include "utils/ntsh_core_defines.h"
#include "../external/Common/ntsh_engine_defines.h"
#include "../external/Common/ntsh_engine_enums.h"
#ifdef NTSH_OS_WINDOWS
#include "module_loaders/module_loaders_windows.h"
#elif NTSH_OS_LINUX
#include "module_loaders/module_loaders_linux.h"
#endif

int main()
{
	ModuleLoader moduleLoader;
#ifdef NTSH_OS_WINDOWS
	NutshellModuleInterface* module = moduleLoader.loadModule("./modules/NutshellModule.dll");
#elif NTSH_OS_LINUX
	NutshellModuleInterface* module = moduleLoader.loadModule("./modules/libNutshellModule.so");
#endif

	NTSH_CORE_INFO(module->getName());
	
	moduleLoader.unloadModule(module);

	return 0;
}
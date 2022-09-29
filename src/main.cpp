#include "utils/ntsh_core_defines.h"
#include "../external/Common/ntsh_engine_defines.h"
#include "../external/Common/ntsh_engine_enums.h"
#include "../external/Common/ntsh_module_interface.h"
#ifdef NTSH_OS_WINDOWS
#include <windows.h>
#elif NTSH_OS_LINUX
#include <dlfcn.h>
#endif

int main()
{
#ifdef NTSH_OS_WINDOWS
	HINSTANCE moduleLibrary = LoadLibrary("./modules/NutshellModule.dll");
#elif NTSH_OS_LINUX
	void* moduleLibrary = dlopen("./modules/NutshellModule.dll", RTLD_LAZY);
#endif

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library NutshellModule.", NTSH_RESULT_UNKNOWN_ERROR);
	}

#ifdef NTSH_OS_WINDOWS
	createModule_t createNutshellModule = (createModule_t)GetProcAddress(moduleLibrary, "createModule");
	if (!createNutshellModule) {
		NTSH_CORE_ERROR("Could not load symbol createModule.", NTSH_RESULT_UNKNOWN_ERROR);
	}
#elif NTSH_OS_LINUX
	createModule_t createNutshellModule = (createModule_t*)dlsym(moduleLibrary, "createModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol createModule.", NTSH_RESULT_UNKNOWN_ERROR);
	}
#endif

#ifdef NTSH_OS_WINDOWS
	destroyModule_t destroyNutshellModule = (destroyModule_t)GetProcAddress(moduleLibrary, "destroyModule");
	if (!createNutshellModule) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_UNKNOWN_ERROR);
	}
#elif NTSH_OS_LINUX
	destroyModule_t destroyNutshellModule = (destroyModule_t*)dlsym(moduleLibrary, "destroyModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_UNKNOWN_ERROR);
	}
#endif

	NutshellModuleInterface* module = createNutshellModule();
	module->init();
	
	destroyNutshellModule(module);

	return 0;
}
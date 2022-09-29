#include "module_loaders_linux.h"
#include "../utils/ntsh_core_defines.h"

NutshellModuleInterface* ModuleLoader::loadModule(const std::string& modulePath) {
	NTSH_CORE_INFO("Loading module from " + modulePath + ".");

	void* moduleLibrary = dlopen("./modules/libNutshellModule.so", RTLD_LAZY);

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createModule_t* createNutshellModule = (createModule_t*)dlsym(moduleLibrary, "createModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol createModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellModuleInterface* module = createNutshellModule();
	m_modules[module] = moduleLibrary;

	return module;
}

void ModuleLoader::unloadModule(NutshellModuleInterface* module) {
	NTSH_CORE_INFO("Unloading module " + module->getName() + ".");

	destroyModule_t* destroyNutshellModule = (destroyModule_t*)dlsym(m_modules[module], "destroyModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyNutshellModule(module);

	if (dlclose(m_modules[module]) != 0) {
		dlsymError = dlerror();
		NTSH_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}
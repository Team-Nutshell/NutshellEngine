#include "module_loaders_windows.h"
#include "../utils/ntsh_core_defines.h"

NutshellModuleInterface* ModuleLoader::loadModule(const std::string& modulePath) {
	NTSH_CORE_INFO("Loading module from " + modulePath + ".");

	HINSTANCE moduleLibrary = LoadLibrary(modulePath.c_str());

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createModule_t createNutshellModule = (createModule_t)GetProcAddress(moduleLibrary, "createModule");
	if (!createNutshellModule) {
		NTSH_CORE_ERROR("Could not load symbol createModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellModuleInterface* module = createNutshellModule();
	m_modules[module] = moduleLibrary;

	return module;
}

void ModuleLoader::unloadModule(NutshellModuleInterface* module) {
	NTSH_CORE_INFO("Unloading module " + module->getName() + ".");

	destroyModule_t destroyNutshellModule = (destroyModule_t)GetProcAddress(m_modules[module], "destroyModule");
	if (!destroyNutshellModule) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyNutshellModule(module);

	if (!FreeLibrary(m_modules[module])) {
		NTSH_CORE_ERROR("Could not unload the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}
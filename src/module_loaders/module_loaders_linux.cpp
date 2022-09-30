#include "module_loaders_linux.h"
#include "../utils/ntsh_core_defines.h"

NutshellGraphicsModuleInterface* ModuleLoader::loadGraphicsModule() {
	const std::string modulePath = "./modules/libNutshellGraphicsModule.so";
	NTSH_CORE_INFO("Loading Graphics module from " + modulePath + ".");

	void* moduleLibrary = dlopen(modulePath.c_str(), RTLD_LAZY);

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createGraphicsModule_t* createModule = (createGraphicsModule_t*)dlsym(moduleLibrary, "createModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol createModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellGraphicsModuleInterface* module = createModule();
	m_graphicsModule = std::make_pair(module, moduleLibrary);

	return module;
}

NutshellPhysicsModuleInterface* ModuleLoader::loadPhysicsModule() {
	const std::string modulePath = "./modules/libNutshellPhysicsModule.so";
	NTSH_CORE_INFO("Loading Physics module from " + modulePath + ".");

	void* moduleLibrary = dlopen(modulePath.c_str(), RTLD_LAZY);

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createPhysicsModule_t* createModule = (createPhysicsModule_t*)dlsym(moduleLibrary, "createModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol createModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellPhysicsModuleInterface* module = createModule();
	m_physicsModule = std::make_pair(module, moduleLibrary);

	return module;
}

NutshellWindowModuleInterface* ModuleLoader::loadWindowModule() {
	const std::string modulePath = "./modules/libNutshellWindowModule.so";
	NTSH_CORE_INFO("Loading Window module from: " + modulePath + ".");

	void* moduleLibrary = dlopen(modulePath.c_str(), RTLD_LAZY);

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createWindowModule_t* createModule = (createWindowModule_t*)dlsym(moduleLibrary, "createModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol createModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellWindowModuleInterface* module = createModule();
	m_windowModule = std::make_pair(module, moduleLibrary);

	return module;
}

NutshellAudioModuleInterface* ModuleLoader::loadAudioModule() {
	const std::string modulePath = "./modules/libNutshellAudioModule.so";
	NTSH_CORE_INFO("Loading Audio module from: " + modulePath + ".");

	void* moduleLibrary = dlopen(modulePath.c_str(), RTLD_LAZY);

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createAudioModule_t* createModule = (createAudioModule_t*)dlsym(moduleLibrary, "createModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol createModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellAudioModuleInterface* module = createModule();
	m_audioModule = std::make_pair(module, moduleLibrary);

	return module;
}

void ModuleLoader::unloadGraphicsModule() {
	NTSH_CORE_INFO("Unloading Graphics module: " + m_graphicsModule.first->getName() + ".");

	destroyGraphicsModule_t* destroyModule = (destroyGraphicsModule_t*)dlsym(m_graphicsModule.second, "destroyModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_graphicsModule.first);

	if (dlclose(m_graphicsModule.second) != 0) {
		dlsymError = dlerror();
		NTSH_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}

void ModuleLoader::unloadPhysicsModule() {
	NTSH_CORE_INFO("Unloading Physics module: " + m_physicsModule.first->getName() + ".");

	destroyPhysicsModule_t* destroyModule = (destroyPhysicsModule_t*)dlsym(m_physicsModule.second, "destroyModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_physicsModule.first);

	if (dlclose(m_physicsModule.second) != 0) {
		dlsymError = dlerror();
		NTSH_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}

void ModuleLoader::unloadWindowModule() {
	NTSH_CORE_INFO("Unloading Window module: " + m_windowModule.first->getName() + ".");

	destroyWindowModule_t* destroyModule = (destroyWindowModule_t*)dlsym(m_windowModule.second, "destroyModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_windowModule.first);

	if (dlclose(m_windowModule.second) != 0) {
		dlsymError = dlerror();
		NTSH_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}

void ModuleLoader::unloadAudioModule() {
	NTSH_CORE_INFO("Unloading Audio module: " + m_audioModule.first->getName() + ".");

	destroyAudioModule_t* destroyModule = (destroyAudioModule_t*)dlsym(m_audioModule.second, "destroyModule");
	const char* dlsymError = dlerror();
	if (dlsymError) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_audioModule.first);

	if (dlclose(m_audioModule.second) != 0) {
		dlsymError = dlerror();
		NTSH_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}
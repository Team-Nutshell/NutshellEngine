#include "module_loaders_windows.h"
#include "../utils/ntsh_core_defines.h"

NutshellGraphicsModuleInterface* ModuleLoader::loadGraphicsModule() {
	const std::string modulePath = "./modules/NutshellGraphicsModule.dll";
	NTSH_CORE_INFO("Loading Graphics module from: " + modulePath + ".");

	HINSTANCE moduleLibrary = LoadLibrary(modulePath.c_str());

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createGraphicsModule_t createModule = (createGraphicsModule_t)GetProcAddress(moduleLibrary, "createModule");
	if (!createModule) {
		NTSH_CORE_ERROR("Could not load symbol createModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellGraphicsModuleInterface* module = createModule();
	m_graphicsModule = std::make_pair(module, moduleLibrary);

	return module;
}

NutshellPhysicsModuleInterface* ModuleLoader::loadPhysicsModule() {
	const std::string modulePath = "./modules/NutshellPhysicsModule.dll";
	NTSH_CORE_INFO("Loading Physics module from: " + modulePath + ".");

	HINSTANCE moduleLibrary = LoadLibrary(modulePath.c_str());

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createPhysicsModule_t createModule = (createPhysicsModule_t)GetProcAddress(moduleLibrary, "createModule");
	if (!createModule) {
		NTSH_CORE_ERROR("Could not load symbol createModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellPhysicsModuleInterface* module = createModule();
	m_physicsModule = std::make_pair(module, moduleLibrary);

	return module;
}

NutshellWindowModuleInterface* ModuleLoader::loadWindowModule() {
	const std::string modulePath = "./modules/NutshellWindowModule.dll";
	NTSH_CORE_INFO("Loading Window module from: " + modulePath + ".");

	HINSTANCE moduleLibrary = LoadLibrary(modulePath.c_str());

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createWindowModule_t createModule = (createWindowModule_t)GetProcAddress(moduleLibrary, "createModule");
	if (!createModule) {
		NTSH_CORE_ERROR("Could not load symbol createModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellWindowModuleInterface* module = createModule();
	m_windowModule = std::make_pair(module, moduleLibrary);

	return module;
}

NutshellAudioModuleInterface* ModuleLoader::loadAudioModule() {
	const std::string modulePath = "./modules/NutshellAudioModule.dll";
	NTSH_CORE_INFO("Loading Audio module from: " + modulePath + ".");

	HINSTANCE moduleLibrary = LoadLibrary(modulePath.c_str());

	if (!moduleLibrary)
	{
		NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}

	createAudioModule_t createModule = (createAudioModule_t)GetProcAddress(moduleLibrary, "createModule");
	if (!createModule) {
		NTSH_CORE_ERROR("Could not load symbol createModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	NutshellAudioModuleInterface* module = createModule();
	m_audioModule = std::make_pair(module, moduleLibrary);

	return module;
}

void ModuleLoader::unloadGraphicsModule() {
	NTSH_CORE_INFO("Unloading Graphics module: " + m_graphicsModule.first->getName() + ".");

	destroyGraphicsModule_t destroyModule = (destroyGraphicsModule_t)GetProcAddress(m_graphicsModule.second, "destroyModule");
	if (!destroyModule) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_graphicsModule.first);

	if (!FreeLibrary(m_graphicsModule.second)) {
		NTSH_CORE_ERROR("Could not unload the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}

void ModuleLoader::unloadPhysicsModule() {
	NTSH_CORE_INFO("Unloading Physics module: " + m_physicsModule.first->getName() + ".");

	destroyPhysicsModule_t destroyModule = (destroyPhysicsModule_t)GetProcAddress(m_physicsModule.second, "destroyModule");
	if (!destroyModule) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_physicsModule.first);

	if (!FreeLibrary(m_physicsModule.second)) {
		NTSH_CORE_ERROR("Could not unload the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}

void ModuleLoader::unloadWindowModule() {
	NTSH_CORE_INFO("Unloading Window module: " + m_windowModule.first->getName() + ".");

	destroyWindowModule_t destroyModule = (destroyWindowModule_t)GetProcAddress(m_windowModule.second, "destroyModule");
	if (!destroyModule) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_windowModule.first);

	if (!FreeLibrary(m_windowModule.second)) {
		NTSH_CORE_ERROR("Could not unload the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}

void ModuleLoader::unloadAudioModule() {
	NTSH_CORE_INFO("Unloading Audio module: " + m_audioModule.first->getName() + ".");

	destroyAudioModule_t destroyModule = (destroyAudioModule_t)GetProcAddress(m_audioModule.second, "destroyModule");
	if (!destroyModule) {
		NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
	}

	destroyModule(m_audioModule.first);

	if (!FreeLibrary(m_audioModule.second)) {
		NTSH_CORE_ERROR("Could not unload the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
	}
}
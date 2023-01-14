#pragma once
#include "../../external/Common/module_interfaces/ntsh_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_graphics_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_physics_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_window_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_audio_module_interface.h"
#include "../utils/ntsh_core_defines.h"
#include <windows.h>
#include <string>
#include <map>
#include <type_traits>

typedef void* (__stdcall* createModule_t)();
typedef void (__stdcall* destroyModule_t)(void*);

class ModuleLoader {
public:
	template <typename T>
	T* loadModule(const std::string& modulePath) {
		NTSH_CORE_INFO("Loading module from: " + modulePath + ".");

		HINSTANCE moduleLibrary = LoadLibrary(modulePath.c_str());
		if (!moduleLibrary) {
			NTSH_CORE_WARNING("Could not load the dynamic library. Error code: " + std::to_string(GetLastError()));
			return nullptr;
		}

		createModule_t createModule = (createModule_t)GetProcAddress(moduleLibrary, "createModule");
		if (!createModule) {
			NTSH_CORE_ERROR("Could not load symbol createModule from dynamic library. Error code: " + std::to_string(GetLastError()), Ntsh::Result::ModuleSymbolLoadError);
		}

		T* module = static_cast<T*>(createModule());

		m_modules[typeid(T).name()] = moduleLibrary;

		NTSH_CORE_INFO("Loaded module \"" + module->getName() + "\".");

		return module;
	}

	template <typename T>
	void unloadModule(T*& module) {
		NTSH_CORE_INFO("Unloading module " + module->getName() + ".");

		destroyModule_t destroyModule = (destroyModule_t)GetProcAddress(m_modules[typeid(T).name()], "destroyModule");
		if (!destroyModule) {
			NTSH_CORE_ERROR("Could not load symbol destroyModule.", Ntsh::Result::ModuleSymbolLoadError);
		}

		destroyModule(module);

		if (!FreeLibrary(m_modules[typeid(T).name()])) {
			NTSH_CORE_ERROR("Could not unload the dynamic library.", Ntsh::Result::ModuleLibraryLoadError);
		}
	}

private:
	std::map<std::string, HINSTANCE> m_modules;
};
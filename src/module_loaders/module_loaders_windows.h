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
		if (!moduleLibrary)
		{
			NTSH_CORE_ERROR("Could not load the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
		}

		createModule_t createModule = (createModule_t)GetProcAddress(moduleLibrary, "createModule");
		if (!createModule) {
			NTSH_CORE_ERROR("Could not load symbol createModule from dynamic library.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
		}

		T* module = static_cast<T*>(createModule());

		m_modules[typeid(T).name()] = moduleLibrary;

		return module;
	}

	template <typename T>
	void unloadModule(T*& module) {
		NTSH_CORE_INFO("Unloading module " + module->getName() + ".");

		destroyModule_t destroyModule = (destroyModule_t)GetProcAddress(m_modules[typeid(T).name()], "destroyModule");
		if (!destroyModule) {
			NTSH_CORE_ERROR("Could not load symbol destroyModule.", NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
		}

		destroyModule(module);

		if (!FreeLibrary(m_modules[typeid(T).name()])) {
			NTSH_CORE_ERROR("Could not unload the dynamic library.", NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
		}
	}

private:
	std::map<std::string, HINSTANCE> m_modules;
};
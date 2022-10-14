#pragma once
#include "../../external/Common/module_interfaces/ntsh_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_graphics_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_physics_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_window_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_audio_module_interface.h"
#include "../utils/ntsh_core_defines.h"
#include <dlfcn.h>
#include <string>
#include <map>
#include <type_traits>

typedef void* createModule_t();
typedef void destroyModule_t(void*);

class ModuleLoader {
public:
	template <typename T>
	T* loadModule(const std::string& modulePath) {
		NTSH_CORE_INFO("Loading module from: \"" + modulePath + "\".");

		void* moduleLibrary = dlopen(modulePath.c_str(), RTLD_LAZY);
		if (!moduleLibrary)
		{
			NTSH_CORE_WARNING("Could not load the dynamic library.");
			return nullptr;
		}

		createModule_t* createModule = (createModule_t*)dlsym(moduleLibrary, "createModule");
		const char* dlsymError = dlerror();
		if (!createModule) {
			NTSH_CORE_ERROR("Could not load symbol \"createModule\" from dynamic library: " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
		}

		T* module = static_cast<T*>(createModule());

		m_modules[typeid(T).name()] = moduleLibrary;

		return module;
	}

	template <typename T>
	void unloadModule(T*& module) {
		NTSH_CORE_INFO("Unloading module \"" + module->getName() + "\".");

		destroyModule_t* destroyModule = (destroyModule_t*)dlsym(m_modules[typeid(T).name()], "destroyModule");
		const char* dlsymError = dlerror();
		if (dlsymError) {
			NTSH_CORE_ERROR("Could not load symbol \"destroyModule\": " + std::string(dlsymError), NTSH_RESULT_MODULE_SYMBOL_LOAD_ERROR);
		}

		destroyModule(module);

		if (dlclose(m_modules[typeid(T).name()]) != 0) {
			NTSH_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NTSH_RESULT_MODULE_LIBRARY_LOAD_ERROR);
		}
	}

private:
	std::map<std::string, void*> m_modules;
};
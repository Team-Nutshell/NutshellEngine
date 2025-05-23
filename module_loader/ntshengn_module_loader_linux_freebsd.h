#pragma once
#include "../utils/ntshengn_core_defines.h"
#include <dlfcn.h>
#include <string>
#include <unordered_map>
#include <type_traits>

typedef void* createModule_t();
typedef void destroyModule_t(void*);

namespace NtshEngn {

	class ModuleLoader {
	public:
		template <typename T>
		T* loadModule(const std::string& modulePath) {
			NTSHENGN_CORE_INFO("Loading module from: \"" + modulePath + "\".");

			m_moduleLibraries[typeid(T).name()] = dlopen(modulePath.c_str(), RTLD_LAZY);
			if (!m_moduleLibraries[typeid(T).name()]) {
				NTSHENGN_CORE_WARNING("Could not load the dynamic library: " + std::string(dlerror()));
				return nullptr;
			}

			createModule_t* createModule = (createModule_t*)dlsym(m_moduleLibraries[typeid(T).name()], "createModule");
			if (!createModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"createModule\" from dynamic library: " + std::string(dlerror()));
			}

			T* module = static_cast<T*>(createModule());

			NTSHENGN_CORE_INFO("Loaded module \"" + module->getName() + "\".");

			return module;
		}

		template <typename T>
		void unloadModule(T* module) {
			NTSHENGN_CORE_INFO("Unloading module \"" + module->getName() + "\".");

			destroyModule_t* destroyModule = (destroyModule_t*)dlsym(m_moduleLibraries[typeid(T).name()], "destroyModule");
			if (!destroyModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"destroyModule\": " + std::string(dlerror()));
			}

			destroyModule(module);

			if (dlclose(m_moduleLibraries[typeid(T).name()]) != 0) {	
				NTSHENGN_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlerror()));
			}
		}

	private:
		std::unordered_map<std::string, void*> m_moduleLibraries;
	};

}

#pragma once
#include "../utils/ntshengn_core_defines.h"
#include "../Common/modules/ntshengn_module_interface.h"
#include <dlfcn.h>
#include <string>
#include <unordered_map>
#include <type_traits>

typedef void* createModule_t();
typedef void destroyModule_t(void*);

namespace NtshEngn {

	class ModuleLoader {
	public:
		ModuleInterface* loadModule(const std::string& modulePath) {
			NTSHENGN_CORE_INFO("Loading module from: \"" + modulePath + "\".");

			m_moduleLibraries[modulePath] = dlopen(modulePath.c_str(), RTLD_LAZY);
			if (!m_moduleLibraries[modulePath]) {
				NTSHENGN_CORE_WARNING("Could not load the dynamic library: " + std::string(dlerror()));
				return nullptr;
			}

			createModule_t* createModule = (createModule_t*)dlsym(m_moduleLibraries[modulePath], "createModule");
			if (!createModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"createModule\" from dynamic library: " + std::string(dlerror()));
			}

			ModuleInterface* module = static_cast<ModuleInterface*>(createModule());

			NTSHENGN_CORE_INFO("Loaded module \"" + module->getName() + "\".");

			return module;
		}

		void unloadModule(ModuleInterface* module, const std::string& modulePath) {
			NTSHENGN_CORE_INFO("Unloading module \"" + module->getName() + "\".");

			destroyModule_t* destroyModule = (destroyModule_t*)dlsym(m_moduleLibraries[modulePath], "destroyModule");
			if (!destroyModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"destroyModule\": " + std::string(dlerror()));
			}

			destroyModule(module);

			if (dlclose(m_moduleLibraries[modulePath]) != 0) {	
				NTSHENGN_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlerror()));
			}
		}

	private:
		std::unordered_map<std::string, void*> m_moduleLibraries;
	};

}

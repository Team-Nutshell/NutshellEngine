#pragma once
#include "../utils/ntshengn_core_defines.h"
#include <dlfcn.h>
#include <string>
#include <map>
#include <type_traits>

typedef void* createModule_t();
typedef void destroyModule_t(void*);

namespace NtshEngn {

	class ModuleLoader {
	public:
		template <typename T>
		T* loadModule(const std::string& modulePath) {
			NTSHENGN_CORE_INFO("Loading module from: \"" + modulePath + "\".");

			void* moduleLibrary = dlopen(modulePath.c_str(), RTLD_LAZY);
			if (!moduleLibrary)
			{
				NTSHENGN_CORE_WARNING("Could not load the dynamic library.");
				return nullptr;
			}

			createModule_t* createModule = (createModule_t*)dlsym(moduleLibrary, "createModule");
			const char* dlsymError = dlerror();
			if (dlsymError) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"createModule\" from dynamic library: " + std::string(dlsymError), NtshEngn::Result::ModuleSymbolLoadError);
			}

			T* module = static_cast<T*>(createModule());

			m_modules[typeid(T).name()] = moduleLibrary;

			NTSHENGN_CORE_INFO("Loaded module \"" + module->getName() + "\".");

			return module;
		}

		template <typename T>
		void unloadModule(T*& module) {
			NTSHENGN_CORE_INFO("Unloading module \"" + module->getName() + "\".");

			destroyModule_t* destroyModule = (destroyModule_t*)dlsym(m_modules[typeid(T).name()], "destroyModule");
			const char* dlsymError = dlerror();
			if (dlsymError) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"destroyModule\": " + std::string(dlsymError), NtshEngn::Result::ModuleSymbolLoadError);
			}

			destroyModule(module);

			if (dlclose(m_modules[typeid(T).name()]) != 0) {
				NTSHENGN_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NtshEngn::Result::ModuleLibraryLoadError);
			}
		}

	private:
		std::map<std::string, void*> m_modules;
	};

}
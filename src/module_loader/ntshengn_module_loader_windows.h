#pragma once
#include "../utils/ntshengn_core_defines.h"
#include <windows.h>
#include <string>
#include <map>
#include <type_traits>

typedef void* (__stdcall* createModule_t)();
typedef void (__stdcall* destroyModule_t)(void*);

namespace NtshEngn {

	class ModuleLoader {
	public:
		template <typename T>
		T* loadModule(const std::string& modulePath) {
			NTSHENGN_CORE_INFO("Loading module from: " + modulePath + ".");

			HINSTANCE moduleLibrary = LoadLibrary(modulePath.c_str());
			if (!moduleLibrary) {
				NTSHENGN_CORE_WARNING("Could not load the dynamic library. Error code: " + std::to_string(GetLastError()));
				return nullptr;
			}

			createModule_t createModule = (createModule_t)GetProcAddress(moduleLibrary, "createModule");
			if (!createModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol createModule from dynamic library. Error code: " + std::to_string(GetLastError()), NtshEngn::Result::ModuleSymbolLoadError);
			}

			T* module = static_cast<T*>(createModule());

			m_modules[typeid(T).name()] = moduleLibrary;

			NTSHENGN_CORE_INFO("Loaded module \"" + module->getName() + "\".");

			return module;
		}

		template <typename T>
		void unloadModule(T*& module) {
			NTSHENGN_CORE_INFO("Unloading module " + module->getName() + ".");

			destroyModule_t destroyModule = (destroyModule_t)GetProcAddress(m_modules[typeid(T).name()], "destroyModule");
			if (!destroyModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol destroyModule.", NtshEngn::Result::ModuleSymbolLoadError);
			}

			destroyModule(module);

			if (!FreeLibrary(m_modules[typeid(T).name()])) {
				NTSHENGN_CORE_ERROR("Could not unload the dynamic library.", NtshEngn::Result::ModuleLibraryLoadError);
			}
		}

	private:
		std::map<std::string, HINSTANCE> m_modules;
	};

}
#pragma once
#include "../utils/ntshengn_core_defines.h"
#include "../Common/modules/ntshengn_module_interface.h"
#include <windows.h>
#undef far
#undef near
#include <string>
#include <unordered_map>
#include <type_traits>

typedef void* (__stdcall* createModule_t)();
typedef void (__stdcall* destroyModule_t)(void*);

namespace NtshEngn {

	class ModuleLoader {
	public:
		ModuleInterface* loadModule(const std::string& modulePath) {
			NTSHENGN_CORE_INFO("Loading module from: " + modulePath + ".");

			m_moduleLibraries[modulePath] = LoadLibrary(modulePath.c_str());
			if (!m_moduleLibraries[modulePath]) {
				NTSHENGN_CORE_WARNING("Could not load the dynamic library. Error code: " + std::to_string(GetLastError()));
				return nullptr;
			}

			createModule_t createModule = (createModule_t)GetProcAddress(m_moduleLibraries[modulePath], "createModule");
			if (!createModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"createModule\" from dynamic library. Error code: " + std::to_string(GetLastError()));
			}

			ModuleInterface* module = static_cast<ModuleInterface*>(createModule());

			NTSHENGN_CORE_INFO("Loaded module \"" + module->getName() + "\".");

			return module;
		}

		void unloadModule(ModuleInterface* module, const std::string& modulePath) {
			NTSHENGN_CORE_INFO("Unloading module " + module->getName() + ".");

			destroyModule_t destroyModule = (destroyModule_t)GetProcAddress(m_moduleLibraries[modulePath], "destroyModule");
			if (!destroyModule) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"destroyModule\".");
			}

			destroyModule(module);

			if (!FreeLibrary(m_moduleLibraries[modulePath])) {
				NTSHENGN_CORE_ERROR("Could not unload the dynamic library.");
			}
		}

	private:
		std::unordered_map<std::string, HINSTANCE> m_moduleLibraries;
	};

}
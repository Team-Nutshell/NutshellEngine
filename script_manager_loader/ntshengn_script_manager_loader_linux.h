#pragma once
#include "../utils/ntshengn_core_defines.h"
#include <dlfcn.h>
#include <string>
#include <unordered_map>
#include <type_traits>

typedef void* createScriptManager_t();
typedef void destroyScriptManager_t(void*);

namespace NtshEngn {

	class ScriptManagerLoader {
	public:
		template <typename T>
		T* loadScriptManager(const std::string& scriptsPath) {
			NTSHENGN_CORE_INFO("Loading script manager from: \"" + scriptsPath + "\".");

			m_scriptManagerLibrary = dlopen(scriptsPath.c_str(), RTLD_LAZY);
			if (!m_scriptManagerLibrary)
			{
				NTSHENGN_CORE_WARNING("Could not load the dynamic library.");
				return nullptr;
			}

			createScriptManager_t* createScriptManager = (createScriptManager_t*)dlsym(m_scriptManagerLibrary, "createScriptManager");
			const char* dlsymError = dlerror();
			if (dlsymError) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"createScriptManager\" from dynamic library: " + std::string(dlsymError), NtshEngn::Result::SymbolLoadError);
			}

			T* scriptManager = static_cast<T*>(createScriptManager());

			NTSHENGN_CORE_INFO("Loaded script manager.");

			return scriptManager;
		}

		template <typename T>
		void unloadScriptManager(T* scriptManager) {
			NTSHENGN_CORE_INFO("Unloading script manager.");

			destroyScriptManager_t* destroyScriptManager = (destroyModule_t*)dlsym(m_scriptManagerLibrary, "destroyScriptManager");
			const char* dlsymError = dlerror();
			if (dlsymError) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"destroyScriptManager\": " + std::string(dlsymError), NtshEngn::Result::SymbolLoadError);
			}

			destroyScriptManager_t(scriptManager);

			if (dlclose(m_scriptManagerLibrary) != 0) {
				NTSHENGN_CORE_ERROR("Could not unload the dynamic library: " + std::string(dlsymError), NtshEngn::Result::LibraryLoadError);
			}
		}

	private:
		void* m_scriptManagerLibrary;
	};

}
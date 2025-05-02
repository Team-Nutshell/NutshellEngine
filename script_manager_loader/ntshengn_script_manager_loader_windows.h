#pragma once
#include "../utils/ntshengn_core_defines.h"
#include <windows.h>
#undef far
#undef near
#include <string>
#include <unordered_map>
#include <type_traits>

typedef void* (__stdcall* createScriptManager_t)();
typedef void (__stdcall* destroyScriptManager_t)(void*);

namespace NtshEngn {

	class ScriptManagerInterface;

	class ScriptManagerLoader {
	public:
		ScriptManagerInterface* loadScriptManager(const std::string& scriptsPath) {
			NTSHENGN_CORE_INFO("Loading script manager from: " + scriptsPath + ".");

			m_scriptManagerLibrary = LoadLibrary(scriptsPath.c_str());
			if (!m_scriptManagerLibrary) {
				NTSHENGN_CORE_WARNING("Could not load the dynamic library. Error code: " + std::to_string(GetLastError()));
				return nullptr;
			}

			createScriptManager_t createScriptManager = (createScriptManager_t)GetProcAddress(m_scriptManagerLibrary, "createScriptManager");
			if (!createScriptManager) {
				NTSHENGN_CORE_ERROR("Could not load symbol \"createScriptManager\" from dynamic library. Error code: " + std::to_string(GetLastError()));
			}

			ScriptManagerInterface* scriptManager = static_cast<ScriptManagerInterface*>(createScriptManager());

			NTSHENGN_CORE_INFO("Loaded script manager.");

			return scriptManager;
		}

		void unloadScriptManager(ScriptManagerInterface* scriptManager) {
			NTSHENGN_CORE_INFO("Unloading script manager.");

			destroyScriptManager_t destroyScriptManager = (destroyScriptManager_t)GetProcAddress(m_scriptManagerLibrary, "destroyScriptManager");
			if (!destroyScriptManager) {
				NTSHENGN_CORE_ERROR("Could not load symbol destroyScriptManager.");
			}

			destroyScriptManager(scriptManager);

			if (!FreeLibrary(m_scriptManagerLibrary)) {
				NTSHENGN_CORE_ERROR("Could not unload the dynamic library.");
			}
		}

	private:
		HINSTANCE m_scriptManagerLibrary;
	};

}
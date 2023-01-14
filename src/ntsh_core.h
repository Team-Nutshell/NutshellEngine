#pragma once
#include "../external/Common/utils/ntsh_engine_defines.h"
#include "../external/Common/utils/ntsh_engine_enums.h"
#include "utils/ntsh_core_defines.h"
#include "asset_loader/asset_loader.h"
#if defined(NTSH_OS_WINDOWS)
#include "module_loader/module_loader_windows.h"
#elif defined(NTSH_OS_LINUX)
#include "module_loader/module_loader_linux.h"
#endif
#include <filesystem>
#include <chrono>

class NutshellCore {
public:
	void loadModules();
	void unloadModules();
	void setModules();

	void init();
	void update();
	void destroy();

	NutshellGraphicsModuleInterface* getGraphicsModule();
	NutshellPhysicsModuleInterface* getPhysicsModule();
	NutshellWindowModuleInterface* getWindowModule();
	NutshellAudioModuleInterface* getAudioModule();

private:
	NutshellGraphicsModuleInterface* m_graphicsModule = nullptr;
	NutshellPhysicsModuleInterface* m_physicsModule = nullptr;
	NutshellWindowModuleInterface* m_windowModule = nullptr;
	NutshellAudioModuleInterface* m_audioModule = nullptr;

	ModuleLoader m_moduleLoader;
};
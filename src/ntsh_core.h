#pragma once
#include "../external/Common/utils/ntsh_engine_defines.h"
#include "../external/Common/utils/ntsh_engine_enums.h"
#include "../external/Common/ecs/ecs_includes.h"
#include "utils/ntsh_core_defines.h"
#if defined(NTSH_OS_WINDOWS)
#include "module_loader/module_loader_windows.h"
#elif defined(NTSH_OS_LINUX)
#include "module_loader/module_loader_linux.h"
#endif
#include <filesystem>
#include <chrono>

class NutshellCore {
public:
	void init();
	void update();
	void destroy();

	NutshellGraphicsModuleInterface* getGraphicsModule();
	NutshellPhysicsModuleInterface* getPhysicsModule();
	NutshellWindowModuleInterface* getWindowModule();
	NutshellAudioModuleInterface* getAudioModule();

	ECS* getECS();

private:
	void loadModules();
	void unloadModules();
	void setModules();

	void initializeECS();
	void setECS();

private:
	NutshellGraphicsModuleInterface* m_graphicsModule = nullptr;
	NutshellPhysicsModuleInterface* m_physicsModule = nullptr;
	NutshellWindowModuleInterface* m_windowModule = nullptr;
	NutshellAudioModuleInterface* m_audioModule = nullptr;

	ModuleLoader m_moduleLoader;

	ECS m_ecs;
};
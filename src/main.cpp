#include "utils/ntsh_core_defines.h"
#include "../external/Common/ntsh_engine_defines.h"
#include "../external/Common/ntsh_engine_enums.h"
#ifdef NTSH_OS_WINDOWS
#include "module_loaders/module_loaders_windows.h"
#elif NTSH_OS_LINUX
#include "module_loaders/module_loaders_linux.h"
#endif

void setModules(NutshellGraphicsModuleInterface* graphicsModule, NutshellPhysicsModuleInterface* physicsModule, NutshellWindowModuleInterface* windowModule, NutshellAudioModuleInterface* audioModule) {
	graphicsModule->setModules(graphicsModule, physicsModule, windowModule, audioModule);
	physicsModule->setModules(graphicsModule, physicsModule, windowModule, audioModule);
	windowModule->setModules(graphicsModule, physicsModule, windowModule, audioModule);
	audioModule->setModules(graphicsModule, physicsModule, windowModule, audioModule);
}

int main()
{
	ModuleLoader moduleLoader;
	NutshellGraphicsModuleInterface* graphicsModule = moduleLoader.loadGraphicsModule();
	NutshellPhysicsModuleInterface* physicsModule = moduleLoader.loadPhysicsModule();
	NutshellWindowModuleInterface* windowModule = moduleLoader.loadWindowModule();
	NutshellAudioModuleInterface* audioModule = moduleLoader.loadAudioModule();

	setModules(graphicsModule, physicsModule, windowModule, audioModule);

	graphicsModule->init();
	physicsModule->init();
	windowModule->init();
	audioModule->init();

	windowModule->setTitle("NutshellEngine Test");

	while (!windowModule->shouldClose()) {
		windowModule->update(0.0);
		audioModule->update(0.0);
		physicsModule->update(0.0);
		graphicsModule->update(0.0);
	}

	graphicsModule->destroy();
	physicsModule->destroy();
	windowModule->destroy();
	audioModule->destroy();
	
	moduleLoader.unloadGraphicsModule();
	moduleLoader.unloadPhysicsModule();
	moduleLoader.unloadWindowModule();
	moduleLoader.unloadAudioModule();

	return 0;
}
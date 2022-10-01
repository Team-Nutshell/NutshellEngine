#include "utils/ntsh_core_defines.h"
#include "../external/Common/ntsh_engine_defines.h"
#include "../external/Common/ntsh_engine_enums.h"
#ifdef NTSH_OS_WINDOWS
#include "module_loaders/module_loaders_windows.h"
#elif NTSH_OS_LINUX
#include "module_loaders/module_loaders_linux.h"
#endif

int main()
{
	ModuleLoader moduleLoader;
	NutshellGraphicsModuleInterface* graphicsModule = moduleLoader.loadGraphicsModule();
	NutshellPhysicsModuleInterface* physicsModule = moduleLoader.loadPhysicsModule();
	NutshellWindowModuleInterface* windowModule = moduleLoader.loadWindowModule();
	NutshellAudioModuleInterface* audioModule = moduleLoader.loadAudioModule();

	graphicsModule->init();
	physicsModule->init();
	windowModule->init();
	audioModule->init();

	while (!windowModule->shouldClose()) {
		windowModule->update(0.0f);
		audioModule->update(0.0f);
		physicsModule->update(0.0f);
		graphicsModule->update(0.0f);
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
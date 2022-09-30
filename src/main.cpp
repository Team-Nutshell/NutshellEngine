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

	NTSH_CORE_INFO(graphicsModule->getName());
	NTSH_CORE_INFO(physicsModule->getName());
	NTSH_CORE_INFO(windowModule->getName());
	NTSH_CORE_INFO(audioModule->getName());
	
	moduleLoader.unloadGraphicsModule();
	moduleLoader.unloadPhysicsModule();
	moduleLoader.unloadWindowModule();
	moduleLoader.unloadAudioModule();

	return 0;
}
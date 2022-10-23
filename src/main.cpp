#include "utils/ntsh_core_defines.h"
#include "../external/Common/utils/ntsh_engine_defines.h"
#include "../external/Common/utils/ntsh_engine_enums.h"
#ifdef NTSH_OS_WINDOWS
#include "module_loader/module_loader_windows.h"
#elif NTSH_OS_LINUX
#include "module_loader/module_loader_linux.h"
#endif
#include "asset_loader/asset_loader.h"

void setModules(NutshellGraphicsModuleInterface* graphicsModule, NutshellPhysicsModuleInterface* physicsModule, NutshellWindowModuleInterface* windowModule, NutshellAudioModuleInterface* audioModule) {
	NTSH_EXECUTE_IF_NOT_NULL(graphicsModule, setModules(graphicsModule, physicsModule, windowModule, audioModule));
	NTSH_EXECUTE_IF_NOT_NULL(physicsModule, setModules(graphicsModule, physicsModule, windowModule, audioModule));
	NTSH_EXECUTE_IF_NOT_NULL(windowModule, setModules(graphicsModule, physicsModule, windowModule, audioModule));
	NTSH_EXECUTE_IF_NOT_NULL(audioModule, setModules(graphicsModule, physicsModule, windowModule, audioModule));
}

int main() {
#ifdef NTSH_OS_WINDOWS
	const std::string graphicsModulePath = "./modules/NutshellGraphicsModule.dll";
	const std::string physicsModulePath = "./modules/NutshellPhysicsModule.dll";
	const std::string windowModulePath = "./modules/NutshellWindowModule.dll";
	const std::string audioModulePath = "./modules/NutshellAudioModule.dll";
#elif NTSH_OS_LINUX
	const std::string graphicsModulePath = "./modules/libNutshellGraphicsModule.so";
	const std::string physicsModulePath = "./modules/libNutshellPhysicsModule.so";
	const std::string windowModulePath = "./modules/libNutshellWindowModule.so";
	const std::string audioModulePath = "./modules/libNutshellAudioModule.so";
#endif

	ModuleLoader moduleLoader;

	NutshellGraphicsModuleInterface* graphicsModule = moduleLoader.loadModule<NutshellGraphicsModuleInterface>(graphicsModulePath);
	NutshellPhysicsModuleInterface* physicsModule = moduleLoader.loadModule<NutshellPhysicsModuleInterface>(physicsModulePath);
	NutshellWindowModuleInterface* windowModule = moduleLoader.loadModule<NutshellWindowModuleInterface>(windowModulePath);
	NutshellAudioModuleInterface* audioModule = moduleLoader.loadModule<NutshellAudioModuleInterface>(audioModulePath);

	setModules(graphicsModule, physicsModule, windowModule, audioModule);

	NTSH_EXECUTE_IF_NOT_NULL(windowModule, init());
	NTSH_EXECUTE_IF_NOT_NULL(graphicsModule, init());
	NTSH_EXECUTE_IF_NOT_NULL(physicsModule, init());
	NTSH_EXECUTE_IF_NOT_NULL(audioModule, init());

	NTSH_EXECUTE_IF_NOT_NULL(windowModule, setTitle("NutshellEngine Test"));

	AssetLoader assetLoader;
	NTSH_UNUSED(assetLoader);
	
	bool applicationClose = false;
	while (!applicationClose) {
		NTSH_EXECUTE_IF_NOT_NULL(windowModule, update(0.0));
		NTSH_EXECUTE_IF_NOT_NULL(audioModule, update(0.0));
		NTSH_EXECUTE_IF_NOT_NULL(physicsModule, update(0.0));
		NTSH_EXECUTE_IF_NOT_NULL(graphicsModule, update(0.0));

		if (windowModule) {
			applicationClose = windowModule->shouldClose();
		}
		else {
			applicationClose = true;
		}
	}

	NTSH_EXECUTE_IF_NOT_NULL(graphicsModule, destroy());
	NTSH_EXECUTE_IF_NOT_NULL(physicsModule, destroy());
	NTSH_EXECUTE_IF_NOT_NULL(windowModule, destroy());
	NTSH_EXECUTE_IF_NOT_NULL(audioModule, destroy());
	
	if (graphicsModule) {
		moduleLoader.unloadModule(graphicsModule);
	}
	if (physicsModule) {
		moduleLoader.unloadModule(physicsModule);
	}
	if (windowModule) {
		moduleLoader.unloadModule(windowModule);
	}
	if (audioModule) {
		moduleLoader.unloadModule(audioModule);
	}

	return 0;
}
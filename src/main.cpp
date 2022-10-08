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
	
	moduleLoader.unloadModule(graphicsModule);
	moduleLoader.unloadModule(physicsModule);
	moduleLoader.unloadModule(windowModule);
	moduleLoader.unloadModule(audioModule);

	return 0;
}
#include "utils/ntsh_core_defines.h"
#include "../external/Common/utils/ntsh_engine_defines.h"
#include "../external/Common/utils/ntsh_engine_enums.h"
#ifdef NTSH_OS_WINDOWS
#include "module_loader/module_loader_windows.h"
#elif NTSH_OS_LINUX
#include "module_loader/module_loader_linux.h"
#endif
#include "asset_loader/asset_loader.h"
#include <filesystem>
#include <chrono>

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

	NutshellGraphicsModuleInterface* graphicsModule = nullptr;
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + graphicsModulePath)) {
		graphicsModule = moduleLoader.loadModule<NutshellGraphicsModuleInterface>(graphicsModulePath);
	}
	NutshellPhysicsModuleInterface* physicsModule = nullptr;
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + physicsModulePath)) {
		physicsModule = moduleLoader.loadModule<NutshellPhysicsModuleInterface>(physicsModulePath);
	}
	NutshellWindowModuleInterface* windowModule = nullptr;
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + windowModulePath)) {
		windowModule = moduleLoader.loadModule<NutshellWindowModuleInterface>(windowModulePath);
	}
	NutshellAudioModuleInterface* audioModule = nullptr;
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + audioModulePath)) {
		audioModule = moduleLoader.loadModule<NutshellAudioModuleInterface>(audioModulePath);
	}

	setModules(graphicsModule, physicsModule, windowModule, audioModule);

	NTSH_EXECUTE_IF_NOT_NULL(windowModule, init());
	NTSH_EXECUTE_IF_NOT_NULL(graphicsModule, init());
	NTSH_EXECUTE_IF_NOT_NULL(physicsModule, init());
	NTSH_EXECUTE_IF_NOT_NULL(audioModule, init());

	NTSH_EXECUTE_IF_NOT_NULL(windowModule, setTitle("NutshellEngine Test"));

	AssetLoader assetLoader;
	NTSH_UNUSED(assetLoader);
	
	double lastFrame = 0.0;
	bool applicationClose = false;
	while (!applicationClose) {
		double currentFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
		double dt = currentFrame - lastFrame;

		NTSH_EXECUTE_IF_NOT_NULL(windowModule, update(dt));
		NTSH_EXECUTE_IF_NOT_NULL(audioModule, update(dt));
		NTSH_EXECUTE_IF_NOT_NULL(physicsModule, update(dt));
		NTSH_EXECUTE_IF_NOT_NULL(graphicsModule, update(dt));

		if (windowModule) {
			applicationClose = windowModule->shouldClose();
		}
		else {
			applicationClose = true;
		}

		lastFrame = currentFrame;
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
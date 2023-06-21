#pragma once
#include "../external/Common/utils/ntshengn_defines.h"
#include "../external/Common/utils/ntshengn_enums.h"
#include "../external/Common/ecs/ntshengn_ecs.h"
#include "../external/Common/asset_manager/ntshengn_asset_manager.h"
#include "scripting/ntshengn_scripting.h"
#include "utils/ntshengn_core_defines.h"
#if defined(NTSHENGN_OS_WINDOWS)
#include "module_loader/ntshengn_module_loader_windows.h"
#elif defined(NTSHENGN_OS_LINUX)
#include "module_loader/ntshengn_module_loader_linux.h"
#endif

namespace NtshEngn {

	class Core {
	public:
		void init();
		void update();
		void destroy();

		GraphicsModuleInterface* getGraphicsModule();
		PhysicsModuleInterface* getPhysicsModule();
		WindowModuleInterface* getWindowModule();
		AudioModuleInterface* getAudioModule();

		ECS* getECS();

		AssetManager* getAssetManager();

	private:
		void loadModules();
		void unloadModules();
		void passModules();

		void initializeECS();
		void passECS();

		void passAssetManager();

	private:
		GraphicsModuleInterface* m_graphicsModule = nullptr;
		PhysicsModuleInterface* m_physicsModule = nullptr;
		WindowModuleInterface* m_windowModule = nullptr;
		AudioModuleInterface* m_audioModule = nullptr;

		Scripting m_scripting;

		ECS m_ecs;

		ModuleLoader m_moduleLoader;

		AssetManager m_assetManager;
	};

}
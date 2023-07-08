#pragma once
#include "../Common/utils/ntshengn_defines.h"
#include "../Common/utils/ntshengn_enums.h"
#include "../Common/ecs/ntshengn_ecs.h"
#include "../Common/asset_manager/ntshengn_asset_manager.h"
#include "../Common/job_system/ntshengn_job_system.h"
#include "frame_limiter/ntshengn_frame_limiter.h"
#include "scripting/ntshengn_scripting.h"
#include "networking/ntshengn_networking.h"
#include "utils/ntshengn_core_defines.h"
#include "module_loader/ntshengn_module_loader.h"

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

		FrameLimiter* getFrameLimiter();

		JobSystem* getJobSystem();

		Networking* getNetworking();

	private:
		void loadModules();
		void unloadModules();
		void passModules();

		void initializeECS();
		void passECS();

		void passAssetManager();

		void passFrameLimiter();

		void initializeJobSystem();
		void passJobSystem();

		void initializeNetworking();
		void passNetworking();

	private:
		GraphicsModuleInterface* m_graphicsModule = nullptr;
		PhysicsModuleInterface* m_physicsModule = nullptr;
		WindowModuleInterface* m_windowModule = nullptr;
		AudioModuleInterface* m_audioModule = nullptr;

		Scripting m_scripting;

		ECS m_ecs;

		ModuleLoader m_moduleLoader;

		AssetManager m_assetManager;

		FrameLimiter m_frameLimiter;

		JobSystem m_jobSystem;

		Networking m_networking;
	};

}
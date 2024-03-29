#pragma once
#include "Common/utils/ntshengn_defines.h"
#include "Common/utils/ntshengn_enums.h"
#include "Common/utils/ntshengn_utils_json.h"
#include "Common/module_interfaces/ntshengn_graphics_module_interface.h"
#include "Common/module_interfaces/ntshengn_physics_module_interface.h"
#include "Common/module_interfaces/ntshengn_window_module_interface.h"
#include "Common/module_interfaces/ntshengn_audio_module_interface.h"
#include "Common/module_interfaces/ntshengn_asset_loader_module_interface.h"
#include "Common/ecs/ntshengn_ecs.h"
#include "Common/asset_manager/ntshengn_asset_manager.h"
#include "Common/job_system/ntshengn_job_system.h"
#include "Common/profiler/ntshengn_profiler.h"
#include "frame_limiter/ntshengn_frame_limiter.h"
#include "scene_manager/ntshengn_scene_manager.h"
#include "scripting/ntshengn_scripting.h"
#include "networking/ntshengn_networking.h"
#include "utils/ntshengn_core_defines.h"
#include "module_loader/ntshengn_module_loader.h"
#include <string>

namespace NtshEngn {

	class Core {
	public:
		void launch(const std::string& optionsFilePath);

		GraphicsModuleInterface* getGraphicsModule();
		PhysicsModuleInterface* getPhysicsModule();
		WindowModuleInterface* getWindowModule();
		AudioModuleInterface* getAudioModule();

		ECS* getECS();

		AssetManager* getAssetManager();

		FrameLimiter* getFrameLimiter();

		JobSystem* getJobSystem();

		Profiler* getProfiler();

		Networking* getNetworking();

		SceneManager* getSceneManager();

	private:
		void init();
		void update();
		void destroy();

		void loadModules();
		void unloadModules();

		void passSystemModules();
		void passAssetLoaderModule();

		void initializeECS();
		void passECS();

		void passAssetManager();

		void passFrameLimiter();

		void initializeJobSystem();
		void passJobSystem();

		void passProfiler();

		void initializeNetworking();
		void passNetworking();

		void passSceneManager();

	private:
		GraphicsModuleInterface* m_graphicsModule = nullptr;
		PhysicsModuleInterface* m_physicsModule = nullptr;
		WindowModuleInterface* m_windowModule = nullptr;
		AudioModuleInterface* m_audioModule = nullptr;
		
		AssetLoaderModuleInterface* m_assetLoaderModule = nullptr;

		Scripting m_scripting;

		ECS m_ecs;

		ModuleLoader m_moduleLoader;

		AssetManager m_assetManager;

		FrameLimiter m_frameLimiter;

		JobSystem m_jobSystem;

		Profiler m_profiler;

		Networking m_networking;

		SceneManager m_sceneManager;
	};

}
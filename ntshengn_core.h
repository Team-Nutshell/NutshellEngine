#pragma once
#include "Common/utils/ntshengn_defines.h"
#include "Common/utils/ntshengn_enums.h"
#include "Common/utils/ntshengn_utils_json.h"
#include "Common/modules/ntshengn_graphics_module_interface.h"
#include "Common/modules/ntshengn_physics_module_interface.h"
#include "Common/modules/ntshengn_window_module_interface.h"
#include "Common/modules/ntshengn_audio_module_interface.h"
#include "Common/modules/ntshengn_asset_loader_module_interface.h"
#include "ecs/ntshengn_ecs.h"
#include "asset_manager/ntshengn_asset_manager.h"
#include "job_system/ntshengn_job_system.h"
#include "profiler/ntshengn_profiler.h"
#include "frame_limiter/ntshengn_frame_limiter.h"
#include "scene_manager/ntshengn_scene_manager.h"
#include "Common/script/ntshengn_script_manager_interface.h"
#include "networking/ntshengn_networking.h"
#include "utils/ntshengn_core_defines.h"
#include "module_loader/ntshengn_module_loader.h"
#include "script_manager_loader/ntshengn_script_manager_loader.h"
#include "scripting/ntshengn_scripting.h"
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

		void loadScripts();
		void unloadScripts();

		void passSystemModules();
		void passAssetLoaderModule();

		void passScriptManager();

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

		ScriptManagerInterface* m_scriptManager = nullptr;
		Scripting m_scripting;

		ECS m_ecs;

		ModuleLoader m_moduleLoader;

		ScriptManagerLoader m_scriptManagerLoader;

		AssetManager m_assetManager;

		FrameLimiter m_frameLimiter;

		JobSystem m_jobSystem;

		Profiler m_profiler;

		Networking m_networking;

		SceneManager m_sceneManager;
	};

}
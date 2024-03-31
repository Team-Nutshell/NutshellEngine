#pragma once
#include "../Common/ecs/ntshengn_ecs.h"
#include "../Common/asset_manager/ntshengn_asset_manager.h"
#include "../Common/job_system/ntshengn_job_system.h"
#include "../Common/profiler/ntshengn_profiler.h"
#include "../Common/frame_limiter/ntshengn_frame_limiter.h"
#include "../Common/networking/ntshengn_networking.h"
#include "../Common/scene_manager/ntshengn_scene_manager.h"
#include "../Common/script/ntshengn_script.h"
#include "../Common/script/ntshengn_script_manager_interface.h"
#include <unordered_map>
#include <set>

namespace NtshEngn {

	class Scripting : public System {
	public:
		void update(double dt);

		void setSystemModules(GraphicsModuleInterface* graphicsModule, PhysicsModuleInterface* physicsModule, WindowModuleInterface* windowModule, AudioModuleInterface* audioModule);
		void setScriptManager(ScriptManagerInterface* scriptManager);
		void setECS(ECS* ecs);
		void setAssetManager(AssetManager* assetManager);
		void setFrameLimiter(FrameLimiter* frameLimiter);
		void setJobSystem(JobSystem* jobSystem);
		void setProfiler(Profiler* profiler);
		void setNetworking(Networking* networking);
		void setSceneManager(SceneManager* sceneManager);

	public:
		void onEntityComponentAdded(Entity entity, Component componentID);
		void onEntityComponentRemoved(Entity entity, Component componentID);

	private:
		GraphicsModuleInterface* m_graphicsModule = nullptr;
		PhysicsModuleInterface* m_physicsModule = nullptr;
		WindowModuleInterface* m_windowModule = nullptr;
		AudioModuleInterface* m_audioModule = nullptr;

		ECS* m_ecs = nullptr;

		AssetManager* m_assetManager = nullptr;

		FrameLimiter* m_frameLimiter = nullptr;

		JobSystem* m_jobSystem = nullptr;

		Profiler* m_profiler = nullptr;

		Networking* m_networking = nullptr;

		SceneManager* m_sceneManager = nullptr;

	private:
		ScriptManagerInterface* m_scriptManager;
		std::unordered_map<Entity, bool> m_entityScriptsJustInitialized;
		std::set<Entity> m_entityScriptsToDestroy;
	};

}
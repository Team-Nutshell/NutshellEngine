#pragma once
#include "../Common/ecs/ntshengn_ecs.h"
#include "../Common/asset_manager/ntshengn_asset_manager.h"
#include "../Common/job_system/ntshengn_job_system.h"
#include "../Common/profiler/ntshengn_profiler.h"
#include "../frame_limiter/ntshengn_frame_limiter.h"
#include "../networking/ntshengn_networking.h"
#include "../scene_manager/ntshengn_scene_manager.h"
#include "ntshengn_script.h"
#include "ntshengn_script_keeper.h"
#include <unordered_map>
#include <set>

namespace NtshEngn {

	class Scripting : public System {
	public:
		void init();
		void update(double dt);

		void setSystemModules(GraphicsModuleInterface* graphicsModule, PhysicsModuleInterface* physicsModule, WindowModuleInterface* windowModule, AudioModuleInterface* audioModule);
		void setECS(ECS* ecs);
		void setAssetManager(AssetManager* assetManager);
		void setFrameLimiter(FrameLimiter* frameLimiter);
		void setJobSystem(JobSystem* jobSystem);
		void setProfiler(Profiler* profiler);
		void setNetworking(Networking* networking);
		void setSceneManager(SceneManager* sceneManager);

		ScriptKeeper* getScriptKeeper();

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
		ScriptKeeper m_scriptKeeper;
		std::unordered_map<Entity, bool> m_entityScriptsJustInitialized;
		std::set<Entity> m_entityScriptsToDestroy;
	};

}
#pragma once
#include "../Common/ecs/ntshengn_ecs.h"
#include "../Common/asset_manager/ntshengn_asset_manager.h"
#include "../Common/job_system/ntshengn_job_system.h"
#include "../frame_limiter/ntshengn_frame_limiter.h"
#include "../networking/ntshengn_networking.h"
#include "../scene_manager/ntshengn_scene_manager.h"
#include <unordered_map>

namespace NtshEngn {

	struct InternalScript {
		Script* script;
		bool justInitialized;
	};

	class Scripting : public System {
	public:
		void update(double dt);

		void setSystemModules(GraphicsModuleInterface* graphicsModule, PhysicsModuleInterface* physicsModule, WindowModuleInterface* windowModule, AudioModuleInterface* audioModule);
		void setECS(ECS* ecs);
		void setAssetManager(AssetManager* assetManager);
		void setFrameLimiter(FrameLimiter* frameLimiter);
		void setJobSystem(JobSystem* jobSystem);
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

		Networking* m_networking = nullptr;

		SceneManager* m_sceneManager = nullptr;

	private:
		std::unordered_map<Entity, InternalScript> m_entityScripts;
		std::set<Entity> m_entityScriptsToDestroy;
	};

}
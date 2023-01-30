#pragma once
#include "../../external/Common/ecs/ntshengn_ecs.h"
#include <unordered_map>

namespace NtshEngn {

	class Scripting : public System {
	public:
		void init();
		void update(double dt);
		void destroy();

		void setModules(GraphicsModuleInterface* graphicsModule, PhysicsModuleInterface* physicsModule, WindowModuleInterface* windowModule, AudioModuleInterface* audioModule);
		void setECS(ECS* ecs);
		void setAssetManager(AssetManager* assetManager);

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

	private:
		std::unordered_map<Entity, Script*> m_entityScripts;
		std::set<Entity> m_entityScriptsToDestroy;
	};

}
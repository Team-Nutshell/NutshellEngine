#include "ntshengn_scripting.h"

void NtshEngn::Scripting::update(double dt) {
	for (auto& entityScript : m_entityScripts) {
		if (m_entityScriptsToDestroy.find(entityScript.first) == m_entityScriptsToDestroy.end()) {
			if (!entityScript.second.justInitialized) {
				entityScript.second.script->update(dt);
			}
			else {
				entityScript.second.justInitialized = false;
			}
		}
	}

	for (auto destroyedEntity : m_entityScriptsToDestroy) {
		m_entityScripts.erase(destroyedEntity);
	}
}

void NtshEngn::Scripting::setSystemModules(GraphicsModuleInterface* graphicsModule, PhysicsModuleInterface* physicsModule, WindowModuleInterface* windowModule, AudioModuleInterface* audioModule) {
	m_graphicsModule = graphicsModule;
	m_physicsModule = physicsModule;
	m_windowModule = windowModule;
	m_audioModule = audioModule;
}

void NtshEngn::Scripting::setECS(ECS* ecs) {
	m_ecs = ecs;
}

void NtshEngn::Scripting::setAssetManager(AssetManager* assetManager) {
	m_assetManager = assetManager;
}

void NtshEngn::Scripting::setFrameLimiter(FrameLimiter* frameLimiter) {
	m_frameLimiter = frameLimiter;
}

void NtshEngn::Scripting::setJobSystem(JobSystem* jobSystem) {
	m_jobSystem = jobSystem;
}

void NtshEngn::Scripting::setNetworking(Networking* networking) {
	m_networking = networking;
}

void NtshEngn::Scripting::setSceneManager(SceneManager* sceneManager) {
	m_sceneManager = sceneManager;
}

void NtshEngn::Scripting::onEntityComponentAdded(Entity entity, Component componentID) {
	if (componentID == m_ecs->getComponentId<Scriptable>()) {
		const Scriptable& entityScript = m_ecs->getComponent<Scriptable>(entity);

		entityScript.script->setEntityID(entity);
		entityScript.script->setSystemModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule);
		entityScript.script->setECS(m_ecs);
		entityScript.script->setAssetManager(m_assetManager);
		entityScript.script->setFrameLimiter(m_frameLimiter);
		entityScript.script->setJobSystem(m_jobSystem);
		entityScript.script->setNetworking(m_networking);
		entityScript.script->setSceneManager(m_sceneManager);

		InternalScript internalScript;
		internalScript.script = entityScript.script.get();
		internalScript.justInitialized = true;
		// A new entity with a script with the same ID as an old entity with a script that has been destroyed this frame has been created
		if (m_entityScriptsToDestroy.find(entity) != m_entityScriptsToDestroy.end()) {
			m_entityScriptsToDestroy.erase(entity);
		}
		m_entityScripts[entity] = internalScript;

		m_entityScripts[entity].script->init();
	}
}

void NtshEngn::Scripting::onEntityComponentRemoved(Entity entity, Component componentID) {
	if (componentID == m_ecs->getComponentId<Scriptable>()) {
		m_entityScripts[entity].script->destroy();

		m_entityScriptsToDestroy.insert(entity);
	}
}

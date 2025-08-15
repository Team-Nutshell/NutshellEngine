#include "ntshengn_scripting.h"

void NtshEngn::Scripting::update(float dt) {
	for (auto& entityScript : m_entityScriptsJustInitialized) {
		if (m_entityScriptsToDestroy.find(entityScript.first) == m_entityScriptsToDestroy.end()) {
			if (!entityScript.second) {
				const Scriptable& entityScriptable = m_ecs->getComponent<Scriptable>(entityScript.first);
				Script* script = static_cast<Script*>(entityScriptable.script);
				script->update(dt);
			}
			else {
				entityScript.second = false;
			}
		}
	}

	for (auto destroyedEntity : m_entityScriptsToDestroy) {
		m_entityScriptsJustInitialized.erase(destroyedEntity);
	}
	m_entityScriptsToDestroy.clear();
}

void NtshEngn::Scripting::setModules(GraphicsModuleInterface* graphicsModule, PhysicsModuleInterface* physicsModule, WindowModuleInterface* windowModule, AudioModuleInterface* audioModule, PlatformModuleInterface* platformModule) {
	m_graphicsModule = graphicsModule;
	m_physicsModule = physicsModule;
	m_windowModule = windowModule;
	m_audioModule = audioModule;
	m_platformModule = platformModule;
}

void NtshEngn::Scripting::setScriptManager(ScriptManagerInterface* scriptManager) {
	m_scriptManager = scriptManager;
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

void NtshEngn::Scripting::setProfiler(Profiler* profiler) {
	m_profiler = profiler;
}

void NtshEngn::Scripting::setNetworking(Networking* networking) {
	m_networking = networking;
}

void NtshEngn::Scripting::setSceneManager(SceneManager* sceneManager) {
	m_sceneManager = sceneManager;
}

void NtshEngn::Scripting::onEntityComponentAdded(Entity entity, Component componentID) {
	if (componentID == m_ecs->getComponentID<Scriptable>()) {
		const Scriptable& entityScriptable = m_ecs->getComponent<Scriptable>(entity);

		Script* script = static_cast<Script*>(entityScriptable.script);
		script->setEntityID(entity);
		script->setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule, m_platformModule);
		script->setScriptManager(m_scriptManager);
		script->setECS(m_ecs);
		script->setAssetManager(m_assetManager);
		script->setFrameLimiter(m_frameLimiter);
		script->setJobSystem(m_jobSystem);
		script->setProfiler(m_profiler);
		script->setNetworking(m_networking);
		script->setSceneManager(m_sceneManager);
		// A new entity with a script with the same ID as an old entity with a script that has been destroyed this frame has been created
		if (m_entityScriptsToDestroy.find(entity) != m_entityScriptsToDestroy.end()) {
			m_entityScriptsToDestroy.erase(entity);
		}
		m_entityScriptsJustInitialized[entity] = true;

		script->init();
	}
}

void NtshEngn::Scripting::onEntityComponentRemoved(Entity entity, Component componentID) {
	if (componentID == m_ecs->getComponentID<Scriptable>()) {
		const Scriptable& scriptable = m_ecs->getComponent<Scriptable>(entity);
		Script* script = static_cast<Script*>(scriptable.script);
		script->destroy();
		m_scriptManager->destroyScript(script);

		m_entityScriptsToDestroy.insert(entity);
	}
}

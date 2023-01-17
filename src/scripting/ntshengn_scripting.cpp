#include "ntshengn_scripting.h"

void NtshEngn::Scripting::init() {

}

void NtshEngn::Scripting::update(double dt) {
	for (Entity entity : m_entities) {
		const Scriptable& entityScript = m_ecs->getComponent<Scriptable>(entity);

		if (m_activeEntities.find(entity) == m_activeEntities.end()) {
			entityScript.script->setEntityID(entity);
			entityScript.script->setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule);
			entityScript.script->setECS(m_ecs);

			entityScript.script->init();

			m_activeEntities.insert(entity);
		}

		entityScript.script->update(dt);
	}

	for (Entity activeEntity : m_activeEntities) {

		if (m_entities.find(activeEntity) == m_entities.end()) {
			const Scriptable& initializedEntityScript = m_ecs->getComponent<Scriptable>(activeEntity);

			initializedEntityScript.script->destroy();

			m_activeEntities.erase(activeEntity);
		}
	}
}

void NtshEngn::Scripting::destroy() {

}

void NtshEngn::Scripting::setModules(GraphicsModuleInterface* graphicsModule, PhysicsModuleInterface* physicsModule, WindowModuleInterface* windowModule, AudioModuleInterface* audioModule) {
	m_graphicsModule = graphicsModule;
	m_physicsModule = physicsModule;
	m_windowModule = windowModule;
	m_audioModule = audioModule;
}

void NtshEngn::Scripting::setECS(ECS* ecs) {
	m_ecs = ecs;
}

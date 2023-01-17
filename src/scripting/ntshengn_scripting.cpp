#include "ntshengn_scripting.h"

void NtshEngn::Scripting::init() {

}

void NtshEngn::Scripting::update(double dt) {
	for (Entity entity : m_entities) {
		const Scriptable& entityScript = m_ecs->getComponent<Scriptable>(entity);

		entityScript.script->update(dt);
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

void NtshEngn::Scripting::onEntityComponentAdded(Entity entity, Component componentID) {
	if (componentID == m_ecs->getComponentId<Scriptable>()) {
		const Scriptable& entityScript = m_ecs->getComponent<Scriptable>(entity);

		entityScript.script->setEntityID(entity);
		entityScript.script->setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule);
		entityScript.script->setECS(m_ecs);

		entityScript.script->init();
	}
}

void NtshEngn::Scripting::onEntityComponentRemoved(Entity entity, Component componentID) {
	if (componentID == m_ecs->getComponentId<Scriptable>()) {
		const Scriptable& entityScript = m_ecs->getComponent<Scriptable>(entity);

		entityScript.script->destroy();
	}
}

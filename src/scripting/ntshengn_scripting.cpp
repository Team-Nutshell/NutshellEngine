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

void NtshEngn::Scripting::setECS(ECS* ecs) {
	m_ecs = ecs;
}

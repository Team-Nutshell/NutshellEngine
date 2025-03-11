#include "ntshengn_ecs.h"

void NtshEngn::ECS::init() {
	m_entityManager = std::make_unique<EntityManager>();
	m_componentManager = std::make_unique<ComponentManager>();
	m_systemManager = std::make_unique<SystemManager>();
}

NtshEngn::Entity NtshEngn::ECS::createEntity() {
	Entity newEntity = m_entityManager->createEntity();
	addComponent(newEntity, Transform{});
			
	return newEntity;
}

NtshEngn::Entity NtshEngn::ECS::createEntity(const std::string& name) {
	Entity newEntity = m_entityManager->createEntity(name);
	addComponent(newEntity, Transform{});
			
	return newEntity;
}

void NtshEngn::ECS::destroyEntity(Entity entity) {
	ComponentMask entityComponents = m_entityManager->getComponents(entity);
	m_systemManager->entityDestroyed(entity, entityComponents);
	m_entityManager->destroyEntity(entity);
	m_componentManager->entityDestroyed(entity);
}

void NtshEngn::ECS::destroyAllEntities() {
	while (!m_entityManager->getExistingEntities().empty()) {
		destroyEntity(*m_entityManager->getExistingEntities().rbegin());
	}
}

void NtshEngn::ECS::destroyNonPersistentEntities() {
	const std::set<Entity>& existingEntities = m_entityManager->getExistingEntities();
	const std::set<Entity>& persistentEntities = m_entityManager->getPersistentEntities();
	std::set<Entity> nonPersistentEntities;
	std::set_difference(existingEntities.begin(), existingEntities.end(), persistentEntities.begin(), persistentEntities.end(), std::inserter(nonPersistentEntities, nonPersistentEntities.begin()));
	while (!nonPersistentEntities.empty()) {
		Entity entityToDestroy = *nonPersistentEntities.rbegin();
		nonPersistentEntities.erase(entityToDestroy);
		destroyEntity(entityToDestroy);
	}
}

bool NtshEngn::ECS::entityExists(Entity entity) {
	return m_entityManager->entityExists(entity);
}

void NtshEngn::ECS::setEntityName(Entity entity, const std::string& name) {
	m_entityManager->setEntityName(entity, name);
}

bool NtshEngn::ECS::entityHasName(Entity entity) {
	return m_entityManager->entityHasName(entity);
}

std::string NtshEngn::ECS::getEntityName(Entity entity) {
	return m_entityManager->getEntityName(entity);
}

NtshEngn::Entity NtshEngn::ECS::findEntityByName(const std::string& name) {
	return m_entityManager->findEntityByName(name);
}

void NtshEngn::ECS::setEntityPersistence(Entity entity, bool persistent) {
	m_entityManager->setEntityPersistence(entity, persistent);
}

bool NtshEngn::ECS::isEntityPersistent(Entity entity) {
	return m_entityManager->isEntityPersistent(entity);
}

void NtshEngn::ECS::addEntityToEntityGroup(Entity entity, const std::string& entityGroupName) {
	m_entityManager->addEntityToEntityGroup(entity, entityGroupName);
}

void NtshEngn::ECS::removeEntityFromEntityGroup(Entity entity, const std::string& entityGroupName) {
	m_entityManager->removeEntityFromEntityGroup(entity, entityGroupName);
}

bool NtshEngn::ECS::entityGroupExists(const std::string& entityGroupName) {
	return m_entityManager->entityGroupExists(entityGroupName);
}

bool NtshEngn::ECS::isEntityInEntityGroup(Entity entity, const std::string& entityGroupName) {
	return m_entityManager->isEntityInEntityGroup(entity, entityGroupName);
}

std::set<NtshEngn::Entity> NtshEngn::ECS::getEntitiesInEntityGroup(const std::string& entityGroupName) {
	return m_entityManager->getEntitiesInEntityGroup(entityGroupName);
}

std::set<std::string> NtshEngn::ECS::getEntityGroupsOfEntity(Entity entity) {
	return m_entityManager->getEntityGroupsOfEntity(entity);
}

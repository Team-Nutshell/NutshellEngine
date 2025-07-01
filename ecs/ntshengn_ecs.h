#pragma once
#include "../Common/ecs/ntshengn_ecs_interface.h"

namespace NtshEngn {

	class ECS : public ECSInterface {
	public:
		void init();

		// Entity
		Entity createEntity();
		Entity createEntity(const std::string& name);

		void destroyEntity(Entity entity);
		void destroyAllEntities();
		void destroyNonPersistentEntities();

		bool entityExists(Entity entity);

		void setEntityName(Entity entity, const std::string& name);
		bool entityHasName(Entity entity);
		std::string getEntityName(Entity entity);
		Entity findEntityByName(const std::string& name);

		void setEntityPersistence(Entity entity, bool persistent);
		bool isEntityPersistent(Entity entity);

		void addEntityToEntityGroup(Entity entity, const std::string& entityGroupName);
		void removeEntityFromEntityGroup(Entity entity, const std::string& entityGroupName);

		bool entityGroupExists(const std::string& entityGroupName);

		bool isEntityInEntityGroup(Entity entity, const std::string& entityGroupName);
		std::set<Entity> getEntitiesInEntityGroup(const std::string& entityGroupName);
		std::set<std::string> getEntityGroupsOfEntity(Entity entity);
	};

}
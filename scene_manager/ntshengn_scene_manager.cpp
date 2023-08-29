#include "ntshengn_scene_manager.h"
#include "../Common/utils/ntshengn_utils_json.h"
#include "../Common/utils/ntshengn_utils_math.h"
#include "../../scripts/ntshengn_scriptable_factory.h"

void NtshEngn::SceneManager::goToScene(const std::string& filePath) {
	JSON json;
	const JSON::Node& sceneRoot = json.read(filePath);

	m_ecs->destroyNonPersistentEntities();

	if (sceneRoot.contains("entities")) {
		const JSON::Node& entitiesNode = sceneRoot["entities"];
		for (size_t i = 0; i < entitiesNode.size(); i++) {
			const JSON::Node& entityNode = entitiesNode[i];

			std::string entityName = "";
			if (entityNode.contains("name")) {
				entityName = entityNode["name"].getString();
			}

			bool entityPersistent = false;
			if (entityNode.contains("isPersistent")) {
				entityPersistent = entityNode["isPersistent"].getBoolean();
			}

			std::vector<Entity> entities;

			// Renderable
			if (entityNode.contains("renderable")) {
				const JSON::Node& renderableNode = entityNode["renderable"];

				if (renderableNode.contains("modelPath")) {
					const JSON::Node& modelPathNode = renderableNode["modelPath"];

					Model* model = m_assetManager->loadModel(modelPathNode.getString());

					entities.resize(model->primitives.size());
					for (size_t j = 0; j < model->primitives.size(); j++) {
						if (entityName != "") {
							entities[j] = m_ecs->createEntity(entityName + std::to_string(j));
						}
						else {
							entities[j] = m_ecs->createEntity();
						}

						m_ecs->setEntityPersistence(entities[j], entityPersistent);

						Renderable renderable;
						renderable.mesh = &model->primitives[j].mesh;
						renderable.material = &model->primitives[j].material;
						m_ecs->addComponent(entities[j], renderable);
					}
				}
			}

			if (entities.empty()) {
				Entity entity;
				if (entityName != "") {
					entity = m_ecs->createEntity(entityName);
				}
				else {
					entity = m_ecs->createEntity();
				}

				m_ecs->setEntityPersistence(entity, entityPersistent);

				entities.push_back(entity);
			}

			for (Entity entity : entities) {
				// Transform
				if (entityNode.contains("transform")) {
					const JSON::Node& transformNode = entityNode["transform"];

					Transform& transform = m_ecs->getComponent<Transform>(entity);
					if (transformNode.contains("position")) {
						const JSON::Node& positionNode = transformNode["position"];

						transform.position = { positionNode[0].getNumber(), positionNode[1].getNumber(), positionNode[2].getNumber() };
					}

					if (transformNode.contains("rotation")) {
						const JSON::Node& rotationNode = transformNode["rotation"];

						transform.rotation = { rotationNode[0].getNumber(), rotationNode[1].getNumber(), rotationNode[2].getNumber() };
					}

					if (transformNode.contains("scale")) {
						const JSON::Node& scaleNode = transformNode["scale"];

						transform.scale = { scaleNode[0].getNumber(), scaleNode[1].getNumber(), scaleNode[2].getNumber() };
					}
				}

				// Camera
				if (entityNode.contains("camera")) {
					const JSON::Node& cameraNode = entityNode["camera"];

					Camera camera;
					if (cameraNode.contains("fov")) {
						const JSON::Node& fovNode = cameraNode["fov"];

						camera.fov = fovNode.getNumber();
					}

					if (cameraNode.contains("nearPlane")) {
						const JSON::Node& nearPlaneNode = cameraNode["nearPlane"];

						camera.nearPlane = nearPlaneNode.getNumber();
					}

					if (cameraNode.contains("farPlane")) {
						const JSON::Node& farPlaneNode = cameraNode["farPlane"];

						camera.farPlane = farPlaneNode.getNumber();
					}

					m_ecs->addComponent(entity, camera);
				}

				// Light
				if (entityNode.contains("light")) {
					const JSON::Node& lightNode = entityNode["light"];

					Light light;
					if (lightNode.contains("type")) {
						const JSON::Node& typeNode = lightNode["type"];

						if (typeNode.getString() == "Directional") {
							light.type = LightType::Directional;
						}
						else if  (typeNode.getString() == "Point") {
							light.type = LightType::Point;
						}
						else if  (typeNode.getString() == "Spot") {
							light.type = LightType::Spot;
						}
					}

					if (lightNode.contains("color")) {
						const JSON::Node& colorNode = lightNode["color"];

						light.color = { colorNode[0].getNumber(), colorNode[1].getNumber(), colorNode[2].getNumber() };
					}

					if (lightNode.contains("cutoff")) {
						const JSON::Node& cutoffNode = lightNode["cutoff"];

						light.cutoff = { cutoffNode[0].getNumber(), cutoffNode[1].getNumber() };
					}

					m_ecs->addComponent(entity, light);
				}

				// Rigidbody
				if (entityNode.contains("rigidbody")) {
					const JSON::Node& rigidbodyNode = entityNode["rigidbody"];

					Rigidbody rigidbody;
					if (rigidbodyNode.contains("isStatic")) {
						const JSON::Node& isStaticNode = rigidbodyNode["isStatic"];

						rigidbody.isStatic = isStaticNode.getBoolean();
					}

					if (rigidbodyNode.contains("isAffectedByConstants")) {
						const JSON::Node& isAffectedByConstantsNode = rigidbodyNode["isAffectedByConstants"];

						rigidbody.isAffectedByConstants = isAffectedByConstantsNode.getBoolean();
					}

					if (rigidbodyNode.contains("mass")) {
						const JSON::Node& massNode = rigidbodyNode["mass"];

						rigidbody.mass = massNode.getNumber();
					}

					if (rigidbodyNode.contains("restitution")) {
						const JSON::Node& restitutionNode = rigidbodyNode["restitution"];

						rigidbody.restitution = restitutionNode.getNumber();
					}

					if (rigidbodyNode.contains("staticFriction")) {
						const JSON::Node& staticFrictionNode = rigidbodyNode["staticFriction"];

						rigidbody.staticFriction = staticFrictionNode.getNumber();
					}

					if (rigidbodyNode.contains("dynamicFriction")) {
						const JSON::Node& dynamicFrictionNode = rigidbodyNode["dynamicFriction"];

						rigidbody.dynamicFriction = dynamicFrictionNode.getNumber();
					}

					m_ecs->addComponent(entity, rigidbody);
				}

				// Collidable
				if (entityNode.contains("collidable")) {
					const JSON::Node& collidableNode = entityNode["collidable"];

					if (collidableNode.contains("type")) {
						const JSON::Node& typeNode = collidableNode["type"];

						if (typeNode.getString() == "AABB") {
							AABBCollidable collidable;
							if (collidableNode.contains("min")) {
								const JSON::Node& minNode = collidableNode["min"];

								collidable.collider.min = { minNode[0].getNumber(), minNode[1].getNumber(), minNode[2].getNumber() };
							}

							if (collidableNode.contains("max")) {
								const JSON::Node& maxNode = collidableNode["max"];

								collidable.collider.max = { maxNode[0].getNumber(), maxNode[1].getNumber(), maxNode[2].getNumber() };
							}

							if (!collidableNode.contains("min") && !collidableNode.contains("max")) {
								// Calculate AABB from Renderable
								if (m_ecs->hasComponent<Renderable>(entity)) {
									const Renderable& renderable = m_ecs->getComponent<Renderable>(entity);
									const std::array<Math::vec3, 2> aabb = m_assetManager->calculateAABB(*renderable.mesh);

									collidable.collider.min = aabb[0];
									collidable.collider.max = aabb[1];
								}
							}

							m_ecs->addComponent(entity, collidable);
						}
						else if (typeNode.getString() == "Sphere") {
							SphereCollidable collidable;
							if (collidableNode.contains("center")) {
								const JSON::Node& centerNode = collidableNode["center"];

								collidable.collider.center = { centerNode[0].getNumber(), centerNode[1].getNumber(), centerNode[2].getNumber() };
							}

							if (collidableNode.contains("radius")) {
								const JSON::Node& radiusNode = collidableNode["radius"];

								collidable.collider.radius = radiusNode.getNumber();
							}

							if (!collidableNode.contains("center") && !collidableNode.contains("radius")) {
								// Calculate sphere from Renderable
								if (m_ecs->hasComponent<Renderable>(entity)) {
									const Renderable& renderable = m_ecs->getComponent<Renderable>(entity);
									const std::array<Math::vec3, 2> aabb = m_assetManager->calculateAABB(*renderable.mesh);

									const Math::vec3 min = aabb[0];

									collidable.collider.center = (aabb[0] + aabb[1]) / 2.0f;
									collidable.collider.radius = (collidable.collider.center - min).length();
								}
							}

							m_ecs->addComponent(entity, collidable);
						}
						else if (typeNode.getString() == "Capsule") {
							CapsuleCollidable collidable;
							if (collidableNode.contains("base")) {
								const JSON::Node& baseNode = collidableNode["base"];

								collidable.collider.base = { baseNode[0].getNumber(), baseNode[1].getNumber(), baseNode[2].getNumber() };
							}

							if (collidableNode.contains("tip")) {
								const JSON::Node& tipNode = collidableNode["tip"];

								collidable.collider.tip = { tipNode[0].getNumber(), tipNode[1].getNumber(), tipNode[2].getNumber() };
							}

							if (collidableNode.contains("radius")) {
								const JSON::Node& radiusNode = collidableNode["radius"];

								collidable.collider.radius = radiusNode.getNumber();
							}

							m_ecs->addComponent(entity, collidable);
						}
					}
				}

				// Scriptable
				if (entityNode.contains("scriptable")) {
					const JSON::Node& scriptableNode = entityNode["scriptable"];

					if (scriptableNode.contains("scriptName")) {
						const JSON::Node& scriptNameNode = scriptableNode["scriptName"];

						m_ecs->addComponent(entity, ScriptableFactory::createScriptable(scriptNameNode.getString()));
					}
				}
			}
		}
	}

	m_currentScenePath = filePath;
}

std::string NtshEngn::SceneManager::getCurrentScenePath() {
	return m_currentScenePath;
}

void NtshEngn::SceneManager::setECS(ECS* ecs) {
	m_ecs = ecs;
}

void NtshEngn::SceneManager::setAssetManager(AssetManager* assetManager) {
	m_assetManager = assetManager;
}
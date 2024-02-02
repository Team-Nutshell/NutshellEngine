#include "ntshengn_scene_manager.h"
#include "../Common/utils/ntshengn_utils_json.h"
#include "../Common/utils/ntshengn_utils_math.h"
#include "../../scripts/ntshengn_scriptable_factory.h"
#include <algorithm>
#include <limits>

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

					if (model) {
						entities.resize(model->primitives.size());
						for (size_t j = 0; j < model->primitives.size(); j++) {
							if (entityName != "") {
								if (model->primitives.size() > 1) {
									entities[j] = m_ecs->createEntity(entityName + "_" + std::to_string(j));
								}
								else {
									entities[j] = m_ecs->createEntity(entityName);
								}
							}
							else {
								entities[j] = m_ecs->createEntity();
							}

							m_ecs->setEntityPersistence(entities[j], entityPersistent);

							Renderable renderable;
							renderable.mesh = &model->primitives[j].mesh;
							renderable.material = model->primitives[j].material;
							m_ecs->addComponent(entities[j], renderable);
						}
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
						else if (typeNode.getString() == "Point") {
							light.type = LightType::Point;
						}
						else if (typeNode.getString() == "Spot") {
							light.type = LightType::Spot;
						}
					}

					if (lightNode.contains("color")) {
						const JSON::Node& colorNode = lightNode["color"];

						light.color = { colorNode[0].getNumber(), colorNode[1].getNumber(), colorNode[2].getNumber() };
					}

					if (lightNode.contains("cutoff")) {
						const JSON::Node& cutoffNode = lightNode["cutoff"];

						light.cutoff = { Math::toRad(cutoffNode[0].getNumber()), Math::toRad(cutoffNode[1].getNumber()) };
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

					if (rigidbodyNode.contains("lockRotation")) {
						const JSON::Node& lockRotationNode = rigidbodyNode["lockRotation"];

						rigidbody.lockRotation = lockRotationNode.getBoolean();
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

					Collidable collidable;
					if (collidableNode.contains("type")) {
						const JSON::Node& typeNode = collidableNode["type"];

						if (typeNode.getString() == "Box") {
							collidable.collider = std::make_unique<ColliderBox>();
							ColliderBox* colliderBox = static_cast<ColliderBox*>(collidable.collider.get());

							if (collidableNode.contains("center")) {
								const JSON::Node& centerNode = collidableNode["center"];

								colliderBox->center = { centerNode[0].getNumber(), centerNode[1].getNumber(), centerNode[2].getNumber() };
							}

							if (collidableNode.contains("halfExtent")) {
								const JSON::Node& halfExtentNode = collidableNode["halfExtent"];

								colliderBox->halfExtent = { halfExtentNode[0].getNumber(), halfExtentNode[1].getNumber(), halfExtentNode[2].getNumber() };
							}

							if (collidableNode.contains("rotation")) {
								const JSON::Node& rotationNode = collidableNode["rotation"];

								colliderBox->rotation = { Math::toRad(rotationNode[0].getNumber()), Math::toRad(rotationNode[1].getNumber()), Math::toRad(rotationNode[2].getNumber()) };
							}

							if (!collidableNode.contains("center") && !collidableNode.contains("halfExtent") && !collidableNode.contains("rotation")) {
								if (m_ecs->hasComponent<Renderable>(entity)) {
									// Calculate box from Renderable
									const Renderable& renderable = m_ecs->getComponent<Renderable>(entity);
									const std::array<Math::vec3, 2> aabb = m_assetManager->calculateAABB(*renderable.mesh);

									colliderBox->center = (aabb[0] + aabb[1]) / 2.0f;
									colliderBox->halfExtent = (aabb[1] - aabb[0]) / 2.0f;
									colliderBox->rotation = Math::vec3(0.0f, 0.0f, 0.0f);
								}
								else {
									// Default box collider
									colliderBox->center = Math::vec3(0.0f, 0.0f, 0.0f);
									colliderBox->halfExtent = Math::vec3(0.5f, 0.5f, 0.5f);
									colliderBox->rotation = Math::vec3(0.0f, 0.0f, 0.0f);
								}
							}
						}
						else if (typeNode.getString() == "Sphere") {
							collidable.collider = std::make_unique<ColliderSphere>();
							ColliderSphere* colliderSphere = static_cast<ColliderSphere*>(collidable.collider.get());

							if (collidableNode.contains("center")) {
								const JSON::Node& centerNode = collidableNode["center"];

								colliderSphere->center = { centerNode[0].getNumber(), centerNode[1].getNumber(), centerNode[2].getNumber() };
							}

							if (collidableNode.contains("radius")) {
								const JSON::Node& radiusNode = collidableNode["radius"];

								colliderSphere->radius = radiusNode.getNumber();
							}

							if (!collidableNode.contains("center") && !collidableNode.contains("radius")) {
								if (m_ecs->hasComponent<Renderable>(entity)) {
									// Calculate sphere from Renderable
									const Renderable& renderable = m_ecs->getComponent<Renderable>(entity);
									const std::array<Math::vec3, 2> aabb = m_assetManager->calculateAABB(*renderable.mesh);

									colliderSphere->center = (aabb[0] + aabb[1]) / 2.0f;
									colliderSphere->radius = (colliderSphere->center - aabb[0]).length();
								}
								else {
									// Default sphere collider
									colliderSphere->center = Math::vec3(0.0f, 0.0f, 0.0f);
									colliderSphere->radius = 0.5f;
								}
							}
						}
						else if (typeNode.getString() == "Capsule") {
							collidable.collider = std::make_unique<ColliderCapsule>();
							ColliderCapsule* colliderCapsule = static_cast<ColliderCapsule*>(collidable.collider.get());

							if (collidableNode.contains("base")) {
								const JSON::Node& baseNode = collidableNode["base"];

								colliderCapsule->base = { baseNode[0].getNumber(), baseNode[1].getNumber(), baseNode[2].getNumber() };
							}

							if (collidableNode.contains("tip")) {
								const JSON::Node& tipNode = collidableNode["tip"];

								colliderCapsule->tip = { tipNode[0].getNumber(), tipNode[1].getNumber(), tipNode[2].getNumber() };
							}

							if (collidableNode.contains("radius")) {
								const JSON::Node& radiusNode = collidableNode["radius"];

								colliderCapsule->radius = radiusNode.getNumber();
							}

							if (!collidableNode.contains("base") && !collidableNode.contains("tip") && !collidableNode.contains("radius")) {
								if (m_ecs->hasComponent<Renderable>(entity)) {
									// Calculate capsule from Renderable
									const Renderable& renderable = m_ecs->getComponent<Renderable>(entity);
									std::array<Math::vec3, 2> aabb = m_assetManager->calculateAABB(*renderable.mesh);

									const Math::vec3 aabbCenter = (aabb[0] + aabb[1]) / 2.0f;
									const Math::vec3 aabbExtent = aabb[1] - aabb[0];
									std::vector<float> extentAsVector = { aabbExtent.x, aabbExtent.y, aabbExtent.z };
									std::vector<float>::iterator extentMaxIt = std::max_element(extentAsVector.begin(), extentAsVector.end());
									uint8_t extentMax = static_cast<uint8_t>(std::distance(extentAsVector.begin(), extentMaxIt));
									extentAsVector[extentMax] = std::numeric_limits<float>::lowest();
									extentMaxIt = std::max_element(extentAsVector.begin(), extentAsVector.end());
									uint8_t extentSecondMax = static_cast<uint8_t>(std::distance(extentAsVector.begin(), extentMaxIt));

									colliderCapsule->radius = std::abs(aabbExtent[extentSecondMax]) / 2.0f;
									colliderCapsule->base = aabbCenter;
									colliderCapsule->base[extentMax] = aabb[0][extentMax] + colliderCapsule->radius;
									colliderCapsule->tip = aabbCenter;
									colliderCapsule->tip[extentMax] -= aabb[1][extentMax] - colliderCapsule->radius;

									if (colliderCapsule->base[extentMax] == colliderCapsule->tip[extentMax]) {
										colliderCapsule->base[extentMax] -= 0.0001f;
										colliderCapsule->tip[extentMax] += 0.0001f;
									}
								}
								else {
									// Default capsule collider
									colliderCapsule->base = Math::vec3(0.0f, 0.25f, 0.0f);
									colliderCapsule->tip = Math::vec3(0.0f, 0.75f, 0.0f);
									colliderCapsule->radius = 0.25f;
								}
							}
						}
					}

					m_ecs->addComponent(entity, collidable);
				}

				// Scriptable
				if (entityNode.contains("scriptable")) {
					const JSON::Node& scriptableNode = entityNode["scriptable"];

					if (scriptableNode.contains("scriptName")) {
						const JSON::Node& scriptNameNode = scriptableNode["scriptName"];

						m_ecs->addComponent(entity, ScriptableFactory::createScriptable(m_scriptKeeper, scriptNameNode.getString()));
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

void NtshEngn::SceneManager::setScriptKeeper(ScriptKeeper* scriptKeeper) {
	m_scriptKeeper = scriptKeeper;
}
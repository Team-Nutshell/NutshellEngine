#include "ntshengn_scene_manager.h"
#include "../utils/ntshengn_utils_json.h"
#include "../utils/ntshengn_utils_math.h"
#include <algorithm>
#include <numeric>
#include <limits>
#include <set>
#include <vector>

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

						transform.rotation = { Math::toRad(rotationNode[0].getNumber()), Math::toRad(rotationNode[1].getNumber()), Math::toRad(rotationNode[2].getNumber()) };
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
					if (cameraNode.contains("forward")) {
						const JSON::Node& forwardNode = cameraNode["forward"];

						camera.forward = { forwardNode[0].getNumber(), forwardNode[1].getNumber(), forwardNode[2].getNumber() };
					}

					if (cameraNode.contains("up")) {
						const JSON::Node& upNode = cameraNode["up"];

						camera.up = { upNode[0].getNumber(), upNode[1].getNumber(), upNode[2].getNumber() };
					}

					if (cameraNode.contains("fov")) {
						const JSON::Node& fovNode = cameraNode["fov"];

						camera.fov = Math::toRad(fovNode.getNumber());
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

					if (lightNode.contains("direction")) {
						const JSON::Node& directionNode = lightNode["direction"];

						light.direction = { directionNode[0].getNumber(), directionNode[1].getNumber(), directionNode[2].getNumber() };
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

					if (rigidbodyNode.contains("inertia")) {
						const JSON::Node& inertiaNode = rigidbodyNode["inertia"];

						rigidbody.inertia = inertiaNode.getNumber();
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

									auto uniquePositionsCmp = [](const Math::vec3& a, const Math::vec3& b) {
										return Math::to_string(a) < Math::to_string(b);
										};
									std::set<Math::vec3, decltype(uniquePositionsCmp)> uniquePositions(uniquePositionsCmp);
									for (size_t j = 0; j < renderable.mesh->vertices.size(); j++) {
										uniquePositions.insert(renderable.mesh->vertices[j].position);
									}

									float size = static_cast<float>(uniquePositions.size());

									const Math::vec3 means = std::reduce(uniquePositions.begin(), uniquePositions.end(), Math::vec3(0.0f, 0.0f, 0.0f), [](Math::vec3 acc, const Math::vec3& val) { return acc + val; }) / size;

									Math::mat3 covarianceMatrix;
									for (const Math::vec3& position : uniquePositions) {
										covarianceMatrix.x.x += (position.x - means.x) * (position.x - means.x);
										covarianceMatrix.y.y += (position.y - means.y) * (position.y - means.y);
										covarianceMatrix.z.z += (position.z - means.z) * (position.z - means.z);
										covarianceMatrix.x.y += (position.x - means.x) * (position.y - means.y);
										covarianceMatrix.x.z += (position.x - means.x) * (position.z - means.z);
										covarianceMatrix.y.z += (position.y - means.y) * (position.z - means.z);
									}
									covarianceMatrix.x.x /= size;
									covarianceMatrix.y.y /= size;
									covarianceMatrix.z.z /= size;
									covarianceMatrix.x.y /= size;
									covarianceMatrix.x.z /= size;
									covarianceMatrix.y.z /= size;

									covarianceMatrix.y.x = covarianceMatrix.x.y;
									covarianceMatrix.z.x = covarianceMatrix.x.z;
									covarianceMatrix.z.y = covarianceMatrix.y.z;

									std::array<std::pair<float, Math::vec3>, 3> eigen = covarianceMatrix.eigen();

									colliderBox->center = means;

									for (const Math::vec3& position : uniquePositions) {
										const Math::vec3 positionMinusCenter = position - colliderBox->center;

										const float extentX = std::abs(Math::dot(eigen[0].second, positionMinusCenter));
										if (extentX > colliderBox->halfExtent.x) {
											colliderBox->halfExtent.x = extentX;
										}

										const float extentY = std::abs(Math::dot(eigen[1].second, positionMinusCenter));
										if (extentY > colliderBox->halfExtent.y) {
											colliderBox->halfExtent.y = extentY;
										}

										const float extentZ = std::abs(Math::dot(eigen[2].second, positionMinusCenter));
										if (extentZ > colliderBox->halfExtent.z) {
											colliderBox->halfExtent.z = extentZ;
										}
									}

									Math::mat4 rotationMatrix = Math::mat4(Math::vec4(eigen[0].second, 0.0f), Math::vec4(eigen[1].second, 0.0f), Math::vec4(eigen[2].second, 0.0f), Math::vec4(0.0f, 0.0f, 0.0f, 1.0f));
									colliderBox->rotation = Math::rotationMatrixToEulerAngles(rotationMatrix);
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

									auto uniquePositionsCmp = [](const Math::vec3& a, const Math::vec3& b) {
										return (a.x < b.x) && (a.y < b.y) && (a.z < b.z);
										};
									std::set<Math::vec3, decltype(uniquePositionsCmp)> uniquePositions(uniquePositionsCmp);
									for (size_t j = 0; j < renderable.mesh->vertices.size(); j++) {
										uniquePositions.insert(renderable.mesh->vertices[i].position);
									}

									float size = static_cast<float>(uniquePositions.size());

									const Math::vec3 means = std::reduce(uniquePositions.begin(), uniquePositions.end(), Math::vec3(0.0f, 0.0f, 0.0f), [](Math::vec3 acc, const Math::vec3& val) { return acc + val; }) / size;

									Math::mat3 covarianceMatrix;
									for (const Math::vec3& position : uniquePositions) {
										covarianceMatrix.x.x += (position.x - means.x) * (position.x - means.x);
										covarianceMatrix.y.y += (position.y - means.y) * (position.y - means.y);
										covarianceMatrix.z.z += (position.z - means.z) * (position.z - means.z);
										covarianceMatrix.x.y += (position.x - means.x) * (position.y - means.y);
										covarianceMatrix.x.z += (position.x - means.x) * (position.z - means.z);
										covarianceMatrix.y.z += (position.y - means.y) * (position.z - means.z);
									}
									covarianceMatrix.x.x /= size;
									covarianceMatrix.y.y /= size;
									covarianceMatrix.z.z /= size;
									covarianceMatrix.x.y /= size;
									covarianceMatrix.x.z /= size;
									covarianceMatrix.y.z /= size;

									covarianceMatrix.y.x = covarianceMatrix.x.y;
									covarianceMatrix.z.x = covarianceMatrix.x.z;
									covarianceMatrix.z.y = covarianceMatrix.y.z;

									std::array<std::pair<float, Math::vec3>, 3> eigen = covarianceMatrix.eigen();
									std::sort(eigen.begin(), eigen.end(), [](const std::pair<float, Math::vec3>& a, const std::pair<float, Math::vec3>& b) {
										return a.first > b.first;
										});

									Math::vec3 capsuleCenter = means;

									float segmentLengthMax = 0.0f;
									for (const Math::vec3& position : uniquePositions) {
										const Math::vec3 positionMinusCenter = position - capsuleCenter;

										const float segmentLength = std::abs(Math::dot(eigen[0].second, positionMinusCenter));
										if (segmentLength > segmentLengthMax) {
											segmentLengthMax = segmentLength;
										}

										const float radius = std::abs(Math::dot(eigen[1].second, positionMinusCenter));
										if (radius > colliderCapsule->radius) {
											colliderCapsule->radius = radius;
										}
									}

									colliderCapsule->base = capsuleCenter - (eigen[0].second * (segmentLengthMax - colliderCapsule->radius));
									colliderCapsule->tip = capsuleCenter + (eigen[0].second * (segmentLengthMax - colliderCapsule->radius));
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
						if (scriptableNode["scriptName"].getString() != "") {
							const JSON::Node& scriptNameNode = scriptableNode["scriptName"];

							m_ecs->addComponent(entity, m_scriptManager->createScriptable(scriptNameNode.getString()));
						}
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

void NtshEngn::SceneManager::setScriptManager(ScriptManagerInterface* scriptManager) {
	m_scriptManager = scriptManager;
}
#include "ntshengn_scene_manager.h"
#if defined(NTSHENGN_OS_LINUX)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
#endif
#include "../utils/ntshengn_utils_json.h"
#if defined(NTSHENGN_OS_LINUX)
#pragma GCC diagnostic pop
#endif
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

			Entity entity = m_ecs->createEntity();

			if (entityNode.contains("name")) {
				std::string entityName = entityNode["name"].getString();
				m_ecs->setEntityName(entity, entityName);
			}

			if (entityNode.contains("isPersistent")) {
				bool entityPersistent = entityNode["isPersistent"].getBoolean();
				m_ecs->setEntityPersistence(entity, entityPersistent);
			}

			// Renderable
			if (entityNode.contains("renderable")) {
				const JSON::Node& renderableNode = entityNode["renderable"];

				Renderable renderable;
				if (renderableNode.contains("modelPath")) {
					const JSON::Node& modelPathNode = renderableNode["modelPath"];

					Model* model = m_assetManager->loadModel(modelPathNode.getString());

					if (renderableNode.contains("primitiveIndex")) {
						const JSON::Node& primitiveIndexNode = renderableNode["primitiveIndex"];

						uint32_t primitiveIndex = static_cast<uint32_t>(primitiveIndexNode.getNumber());
						
						if (model && (primitiveIndex < model->primitives.size())) {
							renderable.mesh = &model->primitives[primitiveIndex].mesh;
							renderable.material = model->primitives[primitiveIndex].material;
						}
					}
				}

				if (renderableNode.contains("materialPath")) {
					const JSON::Node& materialPathNode = renderableNode["materialPath"];

					Material* material = m_assetManager->loadMaterial(materialPathNode.getString());

					renderable.material = *material;
				}

				m_ecs->addComponent(entity, renderable);
			}

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
					else if (typeNode.getString() == "Ambient") {
						light.type = LightType::Ambient;
					}
				}

				if (lightNode.contains("color")) {
					const JSON::Node& colorNode = lightNode["color"];

					light.color = { colorNode[0].getNumber(), colorNode[1].getNumber(), colorNode[2].getNumber() };
				}

				if (lightNode.contains("intensity")) {
					const JSON::Node& intensityNode = lightNode["intensity"];

					light.intensity = intensityNode.getNumber();
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
							// Default box collider
							colliderBox->center = Math::vec3(0.0f, 0.0f, 0.0f);
							colliderBox->halfExtent = Math::vec3(0.5f, 0.5f, 0.5f);
							colliderBox->rotation = Math::vec3(0.0f, 0.0f, 0.0f);
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
							// Default sphere collider
							colliderSphere->center = Math::vec3(0.0f, 0.0f, 0.0f);
							colliderSphere->radius = 0.5f;
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
							// Default capsule collider
							colliderCapsule->base = Math::vec3(0.0f, 0.25f, 0.0f);
							colliderCapsule->tip = Math::vec3(0.0f, 0.75f, 0.0f);
							colliderCapsule->radius = 0.25f;
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

						Scriptable scriptable = m_scriptManager->createScriptable(scriptNameNode.getString());

						if (scriptableNode.contains("editableVariables")) {
							const JSON::Node& editableScriptVariablesNode = scriptableNode["editableVariables"];

							auto& editableScriptVariables = scriptable.script->editableScriptVariables;
							for (auto& editableScriptVariable : editableScriptVariables) {
								if (editableScriptVariablesNode.contains(editableScriptVariable.first)) {
									const JSON::Node& editableScriptVariableNode = editableScriptVariablesNode[editableScriptVariable.first];
									if (editableScriptVariable.second.type == EditableScriptVariableType::Boolean) {
										if (editableScriptVariableNode.getType() == JSON::Type::Boolean) {
											*static_cast<bool*>(editableScriptVariable.second.address) = editableScriptVariableNode.getBoolean();
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Int8) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<int8_t*>(editableScriptVariable.second.address) = static_cast<int8_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Int16) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<int16_t*>(editableScriptVariable.second.address) = static_cast<int16_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Int32) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<int32_t*>(editableScriptVariable.second.address) = static_cast<int32_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Int64) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<int64_t*>(editableScriptVariable.second.address) = static_cast<int64_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Uint8) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<uint8_t*>(editableScriptVariable.second.address) = static_cast<uint8_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Uint16) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<uint16_t*>(editableScriptVariable.second.address) = static_cast<uint16_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Uint32) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<uint32_t*>(editableScriptVariable.second.address) = static_cast<uint32_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Uint64) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<uint64_t*>(editableScriptVariable.second.address) = static_cast<uint64_t>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Float32) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<float*>(editableScriptVariable.second.address) = static_cast<float>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Float64) {
										if (editableScriptVariableNode.getType() == JSON::Type::Number) {
											*static_cast<double*>(editableScriptVariable.second.address) = static_cast<double>(editableScriptVariableNode.getNumber());
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::String) {
										if (editableScriptVariableNode.getType() == JSON::Type::String) {
											*static_cast<std::string*>(editableScriptVariable.second.address) = editableScriptVariableNode.getString();
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Vector2) {
										if ((editableScriptVariableNode.getType() == JSON::Type::Array) && (editableScriptVariableNode.size() == 2)) {
											if (editableScriptVariableNode[0].getType() == JSON::Type::Number) {
												static_cast<Math::vec2*>(editableScriptVariable.second.address)->x = static_cast<float>(editableScriptVariableNode[0].getNumber());
											}
											if (editableScriptVariableNode[1].getType() == JSON::Type::Number) {
												static_cast<Math::vec2*>(editableScriptVariable.second.address)->y = static_cast<float>(editableScriptVariableNode[1].getNumber());
											}
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Vector3) {
										if ((editableScriptVariableNode.getType() == JSON::Type::Array) && (editableScriptVariableNode.size() == 3)) {
											if (editableScriptVariableNode[0].getType() == JSON::Type::Number) {
												static_cast<Math::vec3*>(editableScriptVariable.second.address)->x = static_cast<float>(editableScriptVariableNode[0].getNumber());
											}
											if (editableScriptVariableNode[1].getType() == JSON::Type::Number) {
												static_cast<Math::vec3*>(editableScriptVariable.second.address)->y = static_cast<float>(editableScriptVariableNode[1].getNumber());
											}
											if (editableScriptVariableNode[2].getType() == JSON::Type::Number) {
												static_cast<Math::vec3*>(editableScriptVariable.second.address)->z = static_cast<float>(editableScriptVariableNode[2].getNumber());
											}
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Vector4) {
										if ((editableScriptVariableNode.getType() == JSON::Type::Array) && (editableScriptVariableNode.size() == 4)) {
											if (editableScriptVariableNode[0].getType() == JSON::Type::Number) {
												static_cast<Math::vec4*>(editableScriptVariable.second.address)->x = static_cast<float>(editableScriptVariableNode[0].getNumber());
											}
											if (editableScriptVariableNode[1].getType() == JSON::Type::Number) {
												static_cast<Math::vec4*>(editableScriptVariable.second.address)->y = static_cast<float>(editableScriptVariableNode[1].getNumber());
											}
											if (editableScriptVariableNode[2].getType() == JSON::Type::Number) {
												static_cast<Math::vec4*>(editableScriptVariable.second.address)->z = static_cast<float>(editableScriptVariableNode[2].getNumber());
											}
											if (editableScriptVariableNode[3].getType() == JSON::Type::Number) {
												static_cast<Math::vec4*>(editableScriptVariable.second.address)->w = static_cast<float>(editableScriptVariableNode[3].getNumber());
											}
										}
									}
									else if (editableScriptVariable.second.type == EditableScriptVariableType::Quaternion) {
										if ((editableScriptVariableNode.getType() == JSON::Type::Array) && (editableScriptVariableNode.size() == 4)) {
											if (editableScriptVariableNode[0].getType() == JSON::Type::Number) {
												static_cast<Math::quat*>(editableScriptVariable.second.address)->a = static_cast<float>(editableScriptVariableNode[0].getNumber());
											}
											if (editableScriptVariableNode[1].getType() == JSON::Type::Number) {
												static_cast<Math::quat*>(editableScriptVariable.second.address)->b = static_cast<float>(editableScriptVariableNode[1].getNumber());
											}
											if (editableScriptVariableNode[2].getType() == JSON::Type::Number) {
												static_cast<Math::quat*>(editableScriptVariable.second.address)->c = static_cast<float>(editableScriptVariableNode[2].getNumber());
											}
											if (editableScriptVariableNode[3].getType() == JSON::Type::Number) {
												static_cast<Math::quat*>(editableScriptVariable.second.address)->d = static_cast<float>(editableScriptVariableNode[3].getNumber());
											}
										}
									}
								}
							}
						}

						m_ecs->addComponent(entity, scriptable);
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

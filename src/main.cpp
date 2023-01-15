#include "asset_loader/asset_loader.h"
#include "ntsh_core.h"

void scene(NutshellCore& core) {
	ECS* ecs = core.getECS();

	AssetLoader assetLoader;

	Ntsh::Model* cubeMesh = assetLoader.loadModel("cube.obj");
	Ntsh::Model* roomMesh = assetLoader.loadModel("viking_room.obj");

	Entity cube = core.getECS()->createEntity();

	Transform cubeTransform;
	ecs->addComponent(cube, cubeTransform);

	Renderable cubeRenderable;
	cubeRenderable.mesh = cubeMesh->primitives[0].first;
	cubeRenderable.material = cubeMesh->primitives[0].second;
	ecs->addComponent(cube, cubeRenderable);

	Entity room = core.getECS()->createEntity();

	Transform roomTransform;
	roomTransform.position[0] = 3.0f;
	ecs->addComponent(room, roomTransform);

	Renderable roomRenderable;
	roomRenderable.mesh = roomMesh->primitives[0].first;
	roomRenderable.material = roomMesh->primitives[0].second;
	ecs->addComponent(room, roomRenderable);

	Renderable test = ecs->getComponent<Renderable>(cube);
}

int main() {
	NutshellCore core;

	// Initialize
	core.init();

	// Scene
	scene(core);

	// Loop
	core.update();

	// Destroy
	core.destroy();

	return 0;
}
#include "asset_loader/ntshengn_asset_loader.h"
#include "ntshengn_core.h"

void scene(NtshEngn::Core& core) {
	NtshEngn::ECS* ecs = core.getECS();

	NtshEngn::AssetLoader assetLoader;

	NtshEngn::Model* cubeMesh = assetLoader.loadModel("cube.obj");
	NtshEngn::Model* roomMesh = assetLoader.loadModel("viking_room.obj");

	NtshEngn::Entity cube = core.getECS()->createEntity();

	NtshEngn::Transform cubeTransform;
	ecs->addComponent(cube, cubeTransform);

	NtshEngn::Renderable cubeRenderable;
	cubeRenderable.mesh = cubeMesh->primitives[0].first;
	cubeRenderable.material = cubeMesh->primitives[0].second;
	ecs->addComponent(cube, cubeRenderable);

	NtshEngn::Entity room = core.getECS()->createEntity();

	NtshEngn::Transform roomTransform;
	roomTransform.position[0] = 3.0f;
	ecs->addComponent(room, roomTransform);

	NtshEngn::Renderable roomRenderable;
	roomRenderable.mesh = roomMesh->primitives[0].first;
	roomRenderable.material = roomMesh->primitives[0].second;
	ecs->addComponent(room, roomRenderable);
}

int main() {
	NtshEngn::Core core;

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
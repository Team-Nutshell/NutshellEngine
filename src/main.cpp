#include "asset_loader/ntshengn_asset_loader.h"
#include "ntshengn_core.h"

struct CameraScript : NtshEngn::Script {
	NTSHENGN_SCRIPT(CameraScript);

	void init() {
		std::cout << "create camera" << std::endl;
	}

	void update(double dt) {
		NTSHENGN_UNUSED(dt);
		std::cout << "update camera " << count++ << std::endl;
	}

	void destroy() {
		std::cout << "destroy camera" << std::endl;
	}

private:
	int count = 0;
};

void scene(NtshEngn::Core& core) {
	NtshEngn::ECS* ecs = core.getECS();

	NtshEngn::AssetLoader assetLoader;

	NtshEngn::Model* cubeMesh = assetLoader.loadModel("cube.obj");
	NtshEngn::Model* roomMesh = assetLoader.loadModel("viking_room.obj");

	NtshEngn::Entity cube = core.getECS()->createEntity();

	NtshEngn::Renderable cubeRenderable;
	cubeRenderable.mesh = cubeMesh->primitives[0].first;
	cubeRenderable.material = cubeMesh->primitives[0].second;
	ecs->addComponent(cube, cubeRenderable);

	NtshEngn::Entity room = core.getECS()->createEntity();

	NtshEngn::Transform& roomTransform = ecs->getComponent<NtshEngn::Transform>(room);
	roomTransform.position[0] = 3.0f;

	NtshEngn::Renderable roomRenderable;
	roomRenderable.mesh = roomMesh->primitives[0].first;
	roomRenderable.material = roomMesh->primitives[0].second;
	ecs->addComponent(room, roomRenderable);

	NtshEngn::Entity camera = core.getECS()->createEntity();

	NtshEngn::Transform& cameraTransform = ecs->getComponent<NtshEngn::Transform>(camera);
	cameraTransform.position = { 0.0f, 2.0f, 5.0f };
	cameraTransform.rotation = { 0.0f, 0.0f, -1.0f };
	
	NtshEngn::Camera cameraCamera;
	cameraCamera.fov = 45.0f;
	cameraCamera.nearPlane = 0.03f;
	cameraCamera.farPlane = 100.0f;
	ecs->addComponent(camera, cameraCamera);

	NtshEngn::Scriptable cameraScriptable;
	cameraScriptable.script = std::make_unique<CameraScript>();
	ecs->addComponent(camera, cameraScriptable);
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
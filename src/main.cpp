#include "asset_loader/asset_loader.h"
#include "ntsh_core.h"

void scene(NutshellCore& core) {
	NTSH_UNUSED(core);
}

int main() {
	NutshellCore core;

	// Load modules
	core.loadModules();
	core.setModules();

	// Initialize ECS
	core.initializeECS();
	core.setECS();

	// Initialize modules
	core.init();

	// Scene
	scene(core);

	// Loop
	core.update();

	// Destroy modules
	core.destroy();

	// Unload modules
	core.unloadModules();

	return 0;
}
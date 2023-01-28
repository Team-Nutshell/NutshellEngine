#include "ntshengn_core.h"

void NtshEngn::Core::init() {
	// Load modules
	loadModules();
	setModules();

	// Initialize ECS
	initializeECS();
	setECS();

	// Initialize modules
	NTSHENGN_POINTER_EXECUTE(m_windowModule, init());
	NTSHENGN_POINTER_EXECUTE(m_windowModule, open(1280, 720, "NutshellEngine"));
	m_scripting.init();
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, init());
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, init());
	NTSHENGN_POINTER_EXECUTE(m_audioModule, init());
}

void NtshEngn::Core::update() {
	double lastFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();;
	bool applicationClose = false;
	while (!applicationClose) {
		double currentFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
		double dt = currentFrame - lastFrame;

		// Update modules
		NTSHENGN_POINTER_EXECUTE(m_windowModule, update(dt));
		m_scripting.update(dt);
		NTSHENGN_POINTER_EXECUTE(m_physicsModule, update(dt));
		NTSHENGN_POINTER_EXECUTE(m_audioModule, update(dt));
		NTSHENGN_POINTER_EXECUTE(m_graphicsModule, update(dt));

		applicationClose = m_windowModule ? !m_windowModule->isOpen(NTSHENGN_MAIN_WINDOW) : true;

		lastFrame = currentFrame;
	}
}

void NtshEngn::Core::destroy() {
	// Destroy modules
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, destroy());
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, destroy());
	m_scripting.destroy();
	NTSHENGN_POINTER_EXECUTE(m_windowModule, destroy());
	NTSHENGN_POINTER_EXECUTE(m_audioModule, destroy());

	// Unload module
	unloadModules();
}

NtshEngn::GraphicsModuleInterface* NtshEngn::Core::getGraphicsModule() {
	return m_graphicsModule;
}

NtshEngn::PhysicsModuleInterface* NtshEngn::Core::getPhysicsModule() {
	return m_physicsModule;
}

NtshEngn::WindowModuleInterface* NtshEngn::Core::getWindowModule() {
	return m_windowModule;
}

NtshEngn::AudioModuleInterface* NtshEngn::Core::getAudioModule() {
	return m_audioModule;
}

NtshEngn::ECS* NtshEngn::Core::getECS() {
	return &m_ecs;
}

NtshEngn::AssetManager* NtshEngn::Core::getAssetManager() {
	return &m_assetManager;
}

void NtshEngn::Core::loadModules() {
#if defined(NTSHENGN_OS_WINDOWS)
	const std::string graphicsModulePath = "./modules/NutshellEngine-GraphicsModule.dll";
	const std::string physicsModulePath = "./modules/NutshellEngine-PhysicsModule.dll";
	const std::string windowModulePath = "./modules/NutshellEngine-WindowModule.dll";
	const std::string audioModulePath = "./modules/NutshellEngine-AudioModule.dll";
#elif defined(NTSHENGN_OS_LINUX)
	const std::string graphicsModulePath = "./modules/libNutshellEngine-GraphicsModule.so";
	const std::string physicsModulePath = "./modules/libNutshellEngine-PhysicsModule.so";
	const std::string windowModulePath = "./modules/libNutshellEngine-WindowModule.so";
	const std::string audioModulePath = "./modules/libNutshellEngine-AudioModule.so";
#endif

	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + graphicsModulePath)) {
		m_graphicsModule = m_moduleLoader.loadModule<GraphicsModuleInterface>(graphicsModulePath);
	}
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + physicsModulePath)) {
		m_physicsModule = m_moduleLoader.loadModule<PhysicsModuleInterface>(physicsModulePath);
	}
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + windowModulePath)) {
		m_windowModule = m_moduleLoader.loadModule<WindowModuleInterface>(windowModulePath);
	}
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + audioModulePath)) {
		m_audioModule = m_moduleLoader.loadModule<AudioModuleInterface>(audioModulePath);
	}
}

void NtshEngn::Core::unloadModules() {
	if (m_graphicsModule) {
		m_moduleLoader.unloadModule(m_graphicsModule);
	}
	if (m_physicsModule) {
		m_moduleLoader.unloadModule(m_physicsModule);
	}
	if (m_windowModule) {
		m_moduleLoader.unloadModule(m_windowModule);
	}
	if (m_audioModule) {
		m_moduleLoader.unloadModule(m_audioModule);
	}
}

void NtshEngn::Core::setModules() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	m_scripting.setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
}

void NtshEngn::Core::initializeECS() {
	m_ecs.init();

	m_ecs.registerComponent<Transform>();
	m_ecs.registerComponent<Renderable>();
	m_ecs.registerComponent<Camera>();
	m_ecs.registerComponent<SphereCollidable>();
	m_ecs.registerComponent<AABBCollidable>();
	m_ecs.registerComponent<CapsuleCollidable>();
	m_ecs.registerComponent<Scriptable>();

	if (m_graphicsModule) {
		m_ecs.registerSystem<GraphicsModuleInterface>(m_graphicsModule);
		ComponentMask graphicsComponents;
		graphicsComponents.set(m_ecs.getComponentId<Renderable>());
		graphicsComponents.set(m_ecs.getComponentId<Camera>());
		m_ecs.setSystemComponents<GraphicsModuleInterface>(graphicsComponents);
	}

	if (m_physicsModule) {
		m_ecs.registerSystem<PhysicsModuleInterface>(m_physicsModule);
		ComponentMask physicsComponents;
		physicsComponents.set(m_ecs.getComponentId<SphereCollidable>());
		physicsComponents.set(m_ecs.getComponentId<AABBCollidable>());
		physicsComponents.set(m_ecs.getComponentId<CapsuleCollidable>());
		m_ecs.setSystemComponents<PhysicsModuleInterface>(physicsComponents);
	}

	m_ecs.registerSystem<Scripting>(&m_scripting);
	ComponentMask scriptingComponents;
	scriptingComponents.set(m_ecs.getComponentId<Scriptable>());
	m_ecs.setSystemComponents<Scripting>(scriptingComponents);
}

void NtshEngn::Core::setECS() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setECS(&m_ecs));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setECS(&m_ecs));
	m_scripting.setECS(&m_ecs);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setECS(&m_ecs));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setECS(&m_ecs));
}

void NtshEngn::Core::setAssetManager() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setAssetManager(&m_assetManager));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setAssetManager(&m_assetManager));
	m_scripting.setAssetManager(&m_assetManager);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setAssetManager(&m_assetManager));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setAssetManager(&m_assetManager));
}

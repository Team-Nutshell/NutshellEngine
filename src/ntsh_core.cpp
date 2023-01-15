#include "ntsh_core.h"

void NutshellCore::init() {
	// Load modules
	loadModules();
	setModules();

	// Initialize ECS
	initializeECS();
	setECS();

	// Initialize modules
	NTSH_POINTER_EXECUTE(m_windowModule, init());
	NTSH_POINTER_EXECUTE(m_windowModule, open(1280, 720, "NutshellEngine"));
	NTSH_POINTER_EXECUTE(m_graphicsModule, init());
	NTSH_POINTER_EXECUTE(m_physicsModule, init());
	NTSH_POINTER_EXECUTE(m_audioModule, init());
}

void NutshellCore::update() {
	double lastFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();;
	bool applicationClose = false;
	while (!applicationClose) {
		double currentFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
		double dt = currentFrame - lastFrame;

		// Update modules
		NTSH_POINTER_EXECUTE(m_windowModule, update(dt));
		NTSH_POINTER_EXECUTE(m_physicsModule, update(dt));
		NTSH_POINTER_EXECUTE(m_audioModule, update(dt));
		NTSH_POINTER_EXECUTE(m_graphicsModule, update(dt));

		applicationClose = m_windowModule ? !m_windowModule->isOpen(NTSH_MAIN_WINDOW) : true;

		lastFrame = currentFrame;
	}
}

void NutshellCore::destroy() {
	// Destroy modules
	NTSH_POINTER_EXECUTE(m_graphicsModule, destroy());
	NTSH_POINTER_EXECUTE(m_physicsModule, destroy());
	NTSH_POINTER_EXECUTE(m_windowModule, destroy());
	NTSH_POINTER_EXECUTE(m_audioModule, destroy());

	// Unload module
	unloadModules();
}

NutshellGraphicsModuleInterface* NutshellCore::getGraphicsModule() {
	return m_graphicsModule;
}

NutshellPhysicsModuleInterface* NutshellCore::getPhysicsModule() {
	return m_physicsModule;
}

NutshellWindowModuleInterface* NutshellCore::getWindowModule() {
	return m_windowModule;
}

NutshellAudioModuleInterface* NutshellCore::getAudioModule() {
	return m_audioModule;
}

ECS* NutshellCore::getECS() {
	return &m_ecs;
}

void NutshellCore::loadModules() {
#if defined(NTSH_OS_WINDOWS)
	const std::string graphicsModulePath = "./modules/NutshellGraphicsModule.dll";
	const std::string physicsModulePath = "./modules/NutshellPhysicsModule.dll";
	const std::string windowModulePath = "./modules/NutshellWindowModule.dll";
	const std::string audioModulePath = "./modules/NutshellAudioModule.dll";
#elif defined(NTSH_OS_LINUX)
	const std::string graphicsModulePath = "./modules/libNutshellGraphicsModule.so";
	const std::string physicsModulePath = "./modules/libNutshellPhysicsModule.so";
	const std::string windowModulePath = "./modules/libNutshellWindowModule.so";
	const std::string audioModulePath = "./modules/libNutshellAudioModule.so";
#endif

	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + graphicsModulePath)) {
		m_graphicsModule = m_moduleLoader.loadModule<NutshellGraphicsModuleInterface>(graphicsModulePath);
	}
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + physicsModulePath)) {
		m_physicsModule = m_moduleLoader.loadModule<NutshellPhysicsModuleInterface>(physicsModulePath);
	}
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + windowModulePath)) {
		m_windowModule = m_moduleLoader.loadModule<NutshellWindowModuleInterface>(windowModulePath);
	}
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + audioModulePath)) {
		m_audioModule = m_moduleLoader.loadModule<NutshellAudioModuleInterface>(audioModulePath);
	}
}

void NutshellCore::unloadModules() {
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

void NutshellCore::setModules() {
	NTSH_POINTER_EXECUTE(m_graphicsModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	NTSH_POINTER_EXECUTE(m_physicsModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	NTSH_POINTER_EXECUTE(m_windowModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	NTSH_POINTER_EXECUTE(m_audioModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
}

void NutshellCore::initializeECS() {
	m_ecs.init();

	m_ecs.registerComponent<Transform>();
	m_ecs.registerComponent<Renderable>();

	m_ecs.registerSystem<NutshellGraphicsModuleInterface>(m_graphicsModule);
	ComponentMask graphicsComponents;
	graphicsComponents.set(m_ecs.getComponentId<Transform>());
	graphicsComponents.set(m_ecs.getComponentId<Renderable>());
	m_ecs.setSystemComponents<NutshellGraphicsModuleInterface>(graphicsComponents);
}

void NutshellCore::setECS() {
	NTSH_POINTER_EXECUTE(m_graphicsModule, setECS(&m_ecs));
	NTSH_POINTER_EXECUTE(m_physicsModule, setECS(&m_ecs));
	NTSH_POINTER_EXECUTE(m_windowModule, setECS(&m_ecs));
	NTSH_POINTER_EXECUTE(m_audioModule, setECS(&m_ecs));
}
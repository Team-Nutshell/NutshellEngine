#include "ntshengn_core.h"
#include <filesystem>
#include <chrono>

void NtshEngn::Core::launch(const std::string& optionsFilePath) {
	std::string windowTitle = "NutshellEngine";
	std::string windowIconImagePath = "";
	std::string firstScenePath = "";
	bool startProfiling = false;

	if (std::filesystem::exists(optionsFilePath)) {
		JSON json;
		const JSON::Node& optionsRoot = json.read(optionsFilePath);

		if (optionsRoot.contains("windowTitle")) {
			windowTitle = optionsRoot["windowTitle"].getString();
		}

		if (optionsRoot.contains("windowIconImagePath")) {
			windowIconImagePath = optionsRoot["windowIconImagePath"].getString();
		}

		if (optionsRoot.contains("maxFPS")) {
			m_frameLimiter.setMaxFPS(static_cast<uint32_t>(optionsRoot["maxFPS"].getNumber()));
		}

		if (optionsRoot.contains("firstScenePath")) {
			firstScenePath = optionsRoot["firstScenePath"].getString();
		}

		if (optionsRoot.contains("startProfiling")) {
			startProfiling = optionsRoot["startProfiling"].getBoolean();
			if (startProfiling) {
				m_profiler.start(windowTitle);
			}
		}
	}
	else {
		NTSHENGN_CORE_WARNING("Options file \"" + optionsFilePath + "\" does not exist.");
	}

	// Initialize
	init();

	Image* iconImage = nullptr;
	if (windowIconImagePath != "") {
		iconImage = m_assetManager.loadImage(windowIconImagePath);
	}

	if (m_windowModule && m_windowModule->isWindowOpen(m_windowModule->getMainWindowID())) {
		m_windowModule->setWindowTitle(m_windowModule->getMainWindowID(), windowTitle);

		if (iconImage) {
			m_windowModule->setWindowIcon(m_windowModule->getMainWindowID(), *iconImage);
			m_assetManager.destroyImage(iconImage);
		}
	}

	if (firstScenePath != "") {
		m_sceneManager.goToScene(firstScenePath);
	}

	// Loop
	update();

	// Destroy
	destroy();

	if (startProfiling) {
		NTSHENGN_CORE_INFO("Profiling session: \n" + Profiler::to_string(m_profiler.end()));
	}
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

NtshEngn::FrameLimiter* NtshEngn::Core::getFrameLimiter() {
	return &m_frameLimiter;
}

NtshEngn::JobSystem* NtshEngn::Core::getJobSystem() {
	return &m_jobSystem;
}

NtshEngn::Profiler* NtshEngn::Core::getProfiler() {
	return &m_profiler;
}

NtshEngn::Networking* NtshEngn::Core::getNetworking() {
	return &m_networking;
}

NtshEngn::SceneManager* NtshEngn::Core::getSceneManager() {
	return &m_sceneManager;
}

void NtshEngn::Core::init() {
	// Load modules
	loadModules();

	// Pass System Modules
	passSystemModules();

	// Pass Asset Loader Module
	passAssetLoaderModule();

	// Initialize ECS
	initializeECS();

	// Pass Asset Manager
	passAssetManager();

	// Pass Frame Limiter
	passFrameLimiter();

	// Initialize Job System
	initializeJobSystem();

	// Pass Profiler
	passProfiler();

	// Initialize Networking
	initializeNetworking();

	// Pass Scene Manager
	passSceneManager();

	// Initialize Scripting
	m_scripting.init();

	// Initialize System Modules
	NTSHENGN_POINTER_EXECUTE(m_windowModule, init());
	NTSHENGN_POINTER_EXECUTE(m_windowModule, openWindow(1280, 720, ""));
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, init());
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, init());
	NTSHENGN_POINTER_EXECUTE(m_audioModule, init());
}

void NtshEngn::Core::update() {
	double lastFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
	bool applicationClose = false;
	while (!applicationClose) {
		double currentFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
		double dt = currentFrame - lastFrame;

		// Update
		m_networking.update();
		NTSHENGN_POINTER_EXECUTE(m_windowModule, update(dt));
		m_scripting.update(dt);
		NTSHENGN_POINTER_EXECUTE(m_physicsModule, update(dt));
		NTSHENGN_POINTER_EXECUTE(m_audioModule, update(dt));
		NTSHENGN_POINTER_EXECUTE(m_graphicsModule, update(dt));

		applicationClose = m_windowModule ? !m_windowModule->isWindowOpen(m_windowModule->getMainWindowID()) : true;

		m_frameLimiter.wait(currentFrame);

		lastFrame = currentFrame;
	}
}

void NtshEngn::Core::destroy() {
	// Destroy Networking
	m_networking.destroy();

	// Destroy Job System
	m_jobSystem.destroy();

	// Destroy System Modules
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, destroy());
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, destroy());
	NTSHENGN_POINTER_EXECUTE(m_windowModule, destroy());
	NTSHENGN_POINTER_EXECUTE(m_audioModule, destroy());

	// Unload modules
	unloadModules();
}

void NtshEngn::Core::loadModules() {
#if defined(NTSHENGN_OS_WINDOWS)
	const std::string graphicsModulePath = "./modules/NutshellEngine-GraphicsModule.dll";
	const std::string physicsModulePath = "./modules/NutshellEngine-PhysicsModule.dll";
	const std::string windowModulePath = "./modules/NutshellEngine-WindowModule.dll";
	const std::string audioModulePath = "./modules/NutshellEngine-AudioModule.dll";
	const std::string assetLoaderModulePath = "./modules/NutshellEngine-AssetLoaderModule.dll";
#elif defined(NTSHENGN_OS_LINUX)
	const std::string graphicsModulePath = "./modules/libNutshellEngine-GraphicsModule.so";
	const std::string physicsModulePath = "./modules/libNutshellEngine-PhysicsModule.so";
	const std::string windowModulePath = "./modules/libNutshellEngine-WindowModule.so";
	const std::string audioModulePath = "./modules/libNutshellEngine-AudioModule.so";
	const std::string assetLoaderModulePath = "./modules/libNutshellEngine-AssetLoaderModule.so";
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
	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + assetLoaderModulePath)) {
		m_assetLoaderModule = m_moduleLoader.loadModule<AssetLoaderModuleInterface>(assetLoaderModulePath);
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
	if (m_assetLoaderModule) {
		m_moduleLoader.unloadModule(m_assetLoaderModule);
	}
}

void NtshEngn::Core::passSystemModules() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setSystemModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setSystemModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	m_scripting.setSystemModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setSystemModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setSystemModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule));
}

void NtshEngn::Core::passAssetLoaderModule() {
	m_assetManager.setAssetLoaderModule(m_assetLoaderModule);
}

void NtshEngn::Core::initializeECS() {
	m_ecs.init();

	passECS();

	m_ecs.registerComponent<Transform>();
	m_ecs.registerComponent<Renderable>();
	m_ecs.registerComponent<Camera>();
	m_ecs.registerComponent<Light>();
	m_ecs.registerComponent<Rigidbody>();
	m_ecs.registerComponent<Collidable>();
	m_ecs.registerComponent<Scriptable>();

	if (m_graphicsModule) {
		m_ecs.registerSystem<GraphicsModuleInterface>(m_graphicsModule);
		m_ecs.setSystemComponents<GraphicsModuleInterface>(m_graphicsModule->getComponentMask());
	}

	if (m_physicsModule) {
		m_ecs.registerSystem<PhysicsModuleInterface>(m_physicsModule);
		m_ecs.setSystemComponents<PhysicsModuleInterface>(m_physicsModule->getComponentMask());
	}

	m_ecs.registerSystem<Scripting>(&m_scripting);
	ComponentMask scriptingComponents;
	scriptingComponents.set(m_ecs.getComponentID<Scriptable>());
	m_ecs.setSystemComponents<Scripting>(scriptingComponents);

	if (m_windowModule) {
		m_ecs.registerSystem<WindowModuleInterface>(m_windowModule);
		m_ecs.setSystemComponents<WindowModuleInterface>(m_windowModule->getComponentMask());
	}

	if (m_audioModule) {
		m_ecs.registerSystem<AudioModuleInterface>(m_audioModule);
		m_ecs.setSystemComponents<AudioModuleInterface>(m_audioModule->getComponentMask());
	}
}

void NtshEngn::Core::passECS() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setECS(&m_ecs));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setECS(&m_ecs));
	m_scripting.setECS(&m_ecs);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setECS(&m_ecs));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setECS(&m_ecs));

	m_sceneManager.setECS(&m_ecs);
}

void NtshEngn::Core::passAssetManager() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setAssetManager(&m_assetManager));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setAssetManager(&m_assetManager));
	m_scripting.setAssetManager(&m_assetManager);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setAssetManager(&m_assetManager));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setAssetManager(&m_assetManager));

	m_sceneManager.setAssetManager(&m_assetManager);
}

void NtshEngn::Core::passFrameLimiter() {
	m_scripting.setFrameLimiter(&m_frameLimiter);
}

void NtshEngn::Core::initializeJobSystem() {
	m_jobSystem.init();

	passJobSystem();
}

void NtshEngn::Core::passJobSystem() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setJobSystem(&m_jobSystem));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setJobSystem(&m_jobSystem));
	m_scripting.setJobSystem(&m_jobSystem);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setJobSystem(&m_jobSystem));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setJobSystem(&m_jobSystem));
}

void NtshEngn::Core::passProfiler() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setProfiler(&m_profiler));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setProfiler(&m_profiler));
	m_scripting.setProfiler(&m_profiler);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setProfiler(&m_profiler));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setProfiler(&m_profiler));
	NTSHENGN_POINTER_EXECUTE(m_assetLoaderModule, setProfiler(&m_profiler));
}

void NtshEngn::Core::initializeNetworking() {
	m_networking.init();

	passNetworking();
}

void NtshEngn::Core::passNetworking() {
	m_scripting.setNetworking(&m_networking);
}

void NtshEngn::Core::passSceneManager() {
	m_scripting.setSceneManager(&m_sceneManager);
}
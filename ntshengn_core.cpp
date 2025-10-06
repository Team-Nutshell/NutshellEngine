#include "ntshengn_core.h"
#include "Common/utils/ntshengn_utils_file.h"
#include "Common/utils/ntshengn_utils_json.h"
#include <filesystem>
#include <chrono>

void NtshEngn::Core::run(const std::string& optionsFilePath, const CommandLine& commandLine) {
	m_commandLine = commandLine;

	std::string windowTitle = "NutshellEngine";
	std::string windowIconImagePath = "";
	std::string firstScenePath = "";

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
			if (optionsRoot["startProfiling"].getBoolean()) {
				m_profiler.start(windowTitle);
			}
		}
	}
	else {
		NTSHENGN_CORE_WARNING("Options file \"" + optionsFilePath + "\" does not exist.");
	}

	// Initialize
	m_profiler.startBlock("Init");
	init();
	m_profiler.endBlock();

	Image* iconImage = nullptr;
	if (windowIconImagePath != "") {
		iconImage = m_assetManager.loadImage(windowIconImagePath);
	}

	if (m_windowModule && m_windowModule->isWindowOpen(m_windowModule->getMainWindowID())) {
		m_windowModule->setWindowTitle(m_windowModule->getMainWindowID(), windowTitle);

		if (iconImage) {
			m_windowModule->setWindowIcon(m_windowModule->getMainWindowID(), *iconImage);
			m_assetManager.destroyImage(m_assetManager.getImageName(iconImage));
		}
	}

	if (firstScenePath != "") {
		m_sceneManager.goToScene(firstScenePath);
	}

	// Loop
	m_profiler.startBlock("Update");
	update();
	m_profiler.endBlock();

	// Destroy
	m_profiler.startBlock("Destroy");
	destroy();
	m_profiler.endBlock();

	if (m_profiler.isRunning()) {
		NTSHENGN_CORE_INFO("Profiling session: \n" + ProfilerResultNode::to_string(m_profiler.end()));
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

NtshEngn::PlatformModuleInterface* NtshEngn::Core::getPlatformModule() {
	return m_platformModule;
}

NtshEngn::CommandLine* NtshEngn::Core::getCommandLine() {
	return &m_commandLine;
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
	m_profiler.startBlock("Load modules");
	loadModules();
	m_profiler.endBlock();

	// Load scripts
	m_profiler.startBlock("Load scripts");
	loadScripts();
	m_profiler.endBlock();

	// Pass System Modules
	passSystemModules();

	// Pass Asset Loader Module
	passAssetLoaderModule();

	// Pass Script Manager
	passScriptManager();

	// Pass Command Line
	passCommandLine();

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

	// Initialize System Modules
	m_profiler.startBlock("Init Platform Module");
	NTSHENGN_POINTER_EXECUTE(m_platformModule, init());
	m_profiler.endBlock();
	m_profiler.startBlock("Init Window Module");
	NTSHENGN_POINTER_EXECUTE(m_windowModule, init());
	m_profiler.endBlock();
	NTSHENGN_POINTER_EXECUTE(m_windowModule, openWindow(1280, 720, ""));
	m_profiler.startBlock("Init Graphics Module");
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, init());
	m_profiler.endBlock();
	m_profiler.startBlock("Init Physics Module");
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, init());
	m_profiler.endBlock();
	m_profiler.startBlock("Init Audio Module");
	NTSHENGN_POINTER_EXECUTE(m_audioModule, init());
	m_profiler.endBlock();
	m_profiler.startBlock("Init Asset Loader Module");
	NTSHENGN_POINTER_EXECUTE(m_assetLoaderModule, init());
	m_profiler.endBlock();
}

void NtshEngn::Core::update() {
	double lastFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
	bool applicationClose = false;
	while (!applicationClose) {
		double currentFrame = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
		float dt = static_cast<float>((currentFrame - lastFrame) / 1000.0);

		// Update
		m_profiler.startBlock("Frame");

		m_profiler.startBlock("Update Networking");
		m_networking.update();
		m_profiler.endBlock();
		m_profiler.startBlock("Update Asset Loader Module");
		NTSHENGN_POINTER_EXECUTE(m_assetLoaderModule, update(dt));
		m_profiler.endBlock();
		m_profiler.startBlock("Update Window Module");
		NTSHENGN_POINTER_EXECUTE(m_windowModule, update(dt));
		m_profiler.endBlock();
		m_profiler.startBlock("Update Platform Module");
		NTSHENGN_POINTER_EXECUTE(m_platformModule, update(dt));
		m_profiler.endBlock();
		m_profiler.startBlock("Update Scripting");
		m_scripting.update(dt);
		m_profiler.endBlock();
		m_profiler.startBlock("Update Physics Module");
		NTSHENGN_POINTER_EXECUTE(m_physicsModule, update(dt));
		m_profiler.endBlock();
		m_profiler.startBlock("Update Audio Module");
		NTSHENGN_POINTER_EXECUTE(m_audioModule, update(dt));
		m_profiler.endBlock();
		m_profiler.startBlock("Update Graphics Module");
		NTSHENGN_POINTER_EXECUTE(m_graphicsModule, update(dt));
		m_profiler.endBlock();

		applicationClose = m_windowModule ? !m_windowModule->isWindowOpen(m_windowModule->getMainWindowID()) : true;

		m_profiler.startBlock("Wait Frame Limiter");
		m_frameLimiter.wait(currentFrame);
		m_profiler.endBlock();

		m_profiler.endBlock();

		lastFrame = currentFrame;
	}
}

void NtshEngn::Core::destroy() {
	// Destroy Networking
	m_networking.destroy();

	// Destroy Job System
	m_jobSystem.destroy();

	// Destroy System Modules
	m_profiler.startBlock("Destroy Graphics Module");
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, destroy());
	m_profiler.endBlock();
	m_profiler.startBlock("Destroy Physics Module");
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, destroy());
	m_profiler.endBlock();
	m_profiler.startBlock("Destroy Window Module");
	NTSHENGN_POINTER_EXECUTE(m_windowModule, destroy());
	m_profiler.endBlock();
	m_profiler.startBlock("Destroy Audio Module");
	NTSHENGN_POINTER_EXECUTE(m_audioModule, destroy());
	m_profiler.endBlock();
	m_profiler.startBlock("Destroy Asset Loader Module");
	NTSHENGN_POINTER_EXECUTE(m_assetLoaderModule, destroy());
	m_profiler.endBlock();
	m_profiler.startBlock("Destroy Platform Module");
	NTSHENGN_POINTER_EXECUTE(m_platformModule, destroy());
	m_profiler.endBlock();

	// Unload scripts
	m_profiler.startBlock("Unload scripts");
	unloadScripts();
	m_profiler.endBlock();

	// Unload modules
	m_profiler.startBlock("Unload modules");
	unloadModules();
	m_profiler.endBlock();
}

void NtshEngn::Core::loadModules() {
#if defined(NTSHENGN_OS_WINDOWS)
	const std::string dynamicLibraryExtension = "dll";
#elif defined(NTSHENGN_OS_LINUX) || defined(NTSHENGN_OS_FREEBSD)
	const std::string dynamicLibraryExtension = "so";
#endif

	if (std::filesystem::exists("modules")) {
		const std::filesystem::path modulesDirectory{ "modules" };
		std::vector<std::string> sortedModulePaths;
		for (const std::filesystem::directory_entry& moduleEntry : std::filesystem::directory_iterator(modulesDirectory)) {
			if (!moduleEntry.is_directory() && (File::extension(moduleEntry.path().string()) == dynamicLibraryExtension)) {
				std::string modulePath = moduleEntry.path().string();
				sortedModulePaths.push_back(modulePath);
			}
		}

		std::sort(sortedModulePaths.begin(), sortedModulePaths.end());
		for (const std::string& modulePath : sortedModulePaths) {
			ModuleInterface* module = m_moduleLoader.loadModule(modulePath);
			if ((module->getType() == ModuleType::Graphics) && !m_graphicsModule) {
				m_graphicsModule = static_cast<GraphicsModuleInterface*>(module);
				m_graphicsModulePath = modulePath;
			}
			else if ((module->getType() == ModuleType::Physics) && !m_physicsModule) {
				m_physicsModule = static_cast<PhysicsModuleInterface*>(module);
				m_physicsModulePath = modulePath;
			}
			else if ((module->getType() == ModuleType::Window) && !m_windowModule) {
				m_windowModule = static_cast<WindowModuleInterface*>(module);
				m_windowModulePath = modulePath;
			}
			else if ((module->getType() == ModuleType::Audio) && !m_audioModule) {
				m_audioModule = static_cast<AudioModuleInterface*>(module);
				m_audioModulePath = modulePath;
			}
			else if ((module->getType() == ModuleType::AssetLoader) && !m_assetLoaderModule) {
				m_assetLoaderModule = static_cast<AssetLoaderModuleInterface*>(module);
				m_assetLoaderModulePath = modulePath;
			}
			else if ((module->getType() == ModuleType::Platform) && !m_platformModule) {
				m_platformModule = static_cast<PlatformModuleInterface*>(module);
				m_platformModulePath = modulePath;
			}
			else {
				m_moduleLoader.unloadModule(module, modulePath);
			}
		}
	}
}

void NtshEngn::Core::unloadModules() {
	if (m_graphicsModule) {
		m_moduleLoader.unloadModule(m_graphicsModule, m_graphicsModulePath);
	}
	if (m_physicsModule) {
		m_moduleLoader.unloadModule(m_physicsModule, m_physicsModulePath);
	}
	if (m_windowModule) {
		m_moduleLoader.unloadModule(m_windowModule, m_windowModulePath);
	}
	if (m_audioModule) {
		m_moduleLoader.unloadModule(m_audioModule, m_audioModulePath);
	}
	if (m_assetLoaderModule) {
		m_moduleLoader.unloadModule(m_assetLoaderModule, m_assetLoaderModulePath);
	}
	if (m_platformModule) {
		m_moduleLoader.unloadModule(m_platformModule, m_platformModulePath);
	}
}

void NtshEngn::Core::loadScripts() {
#if defined(NTSHENGN_OS_WINDOWS)
	const std::string scriptsPath = "./NutshellEngine-Scripts.dll";
#elif defined(NTSHENGN_OS_LINUX) || defined(NTSHENGN_OS_FREEBSD)
	const std::string scriptsPath = "./libNutshellEngine-Scripts.so";
#endif

	if (std::filesystem::exists(std::filesystem::current_path().string() + "/" + scriptsPath)) {
		m_scriptManager = m_scriptManagerLoader.loadScriptManager(scriptsPath);
	}
}

void NtshEngn::Core::unloadScripts() {
	if (m_scriptManager) {
		m_scriptManagerLoader.unloadScriptManager(m_scriptManager);
	}
}

void NtshEngn::Core::passSystemModules() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule, m_platformModule));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule, m_platformModule));
	m_scripting.setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule, m_platformModule);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule, m_platformModule));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setModules(m_graphicsModule, m_physicsModule, m_windowModule, m_audioModule, m_platformModule));
}

void NtshEngn::Core::passAssetLoaderModule() {
	m_assetManager.setAssetLoaderModule(m_assetLoaderModule);
}

void NtshEngn::Core::passScriptManager() {
	m_scripting.setScriptManager(m_scriptManager);

	m_sceneManager.setScriptManager(m_scriptManager);
}

void NtshEngn::Core::passCommandLine() {
	NTSHENGN_POINTER_EXECUTE(m_graphicsModule, setCommandLine(&m_commandLine));
	NTSHENGN_POINTER_EXECUTE(m_physicsModule, setCommandLine(&m_commandLine));
	m_scripting.setCommandLine(&m_commandLine);
	NTSHENGN_POINTER_EXECUTE(m_windowModule, setCommandLine(&m_commandLine));
	NTSHENGN_POINTER_EXECUTE(m_audioModule, setCommandLine(&m_commandLine));
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
	m_ecs.registerComponent<SoundListener>();
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
	NTSHENGN_POINTER_EXECUTE(m_assetLoaderModule, setAssetManager(&m_assetManager));

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
	NTSHENGN_POINTER_EXECUTE(m_platformModule, setProfiler(&m_profiler));
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
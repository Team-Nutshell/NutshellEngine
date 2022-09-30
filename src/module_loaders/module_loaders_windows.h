#pragma once
#include "../../external/Common/module_interfaces/ntsh_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_graphics_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_physics_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_window_module_interface.h"
#include "../../external/Common/module_interfaces/ntsh_audio_module_interface.h"
#include <windows.h>
#include <string>
#include <utility>

class ModuleLoader {
public:
	NutshellGraphicsModuleInterface* loadGraphicsModule();
	NutshellPhysicsModuleInterface* loadPhysicsModule();
	NutshellWindowModuleInterface* loadWindowModule();
	NutshellAudioModuleInterface* loadAudioModule();
	void unloadGraphicsModule();
	void unloadPhysicsModule();
	void unloadWindowModule();
	void unloadAudioModule();

private:
	std::pair<NutshellGraphicsModuleInterface*, HINSTANCE> m_graphicsModule;
	std::pair<NutshellPhysicsModuleInterface*, HINSTANCE> m_physicsModule;
	std::pair<NutshellWindowModuleInterface*, HINSTANCE> m_windowModule;
	std::pair<NutshellAudioModuleInterface*, HINSTANCE> m_audioModule;
};
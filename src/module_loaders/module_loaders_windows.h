#pragma once
#include "../../external/Common/ntsh_module_interface.h"
#include <windows.h>
#include <string>
#include <unordered_map>

class ModuleLoader {
public:
	NutshellModuleInterface* loadModule(const std::string& modulePath);
	void unloadModule(NutshellModuleInterface* module);

private:
	std::unordered_map<NutshellModuleInterface*, HINSTANCE> m_modules;
};
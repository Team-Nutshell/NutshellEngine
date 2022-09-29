#pragma once
#include "../../external/Common/ntsh_module_interface.h"
#include <dlfcn.h>
#include <string>
#include <unordered_map>

class ModuleLoader {
public:
	NutshellModuleInterface* loadModule(const std::string& modulePath);
	void unloadModule(NutshellModuleInterface* module);

private:
	std::unordered_map<NutshellModuleInterface*, void*> m_modules;
};
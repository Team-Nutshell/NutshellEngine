#pragma once
#include "../Common/scene_manager/ntshengn_scene_manager_interface.h"
#include "../asset_manager/ntshengn_asset_manager.h"
#include "../ecs/ntshengn_ecs.h"
#include "../script/ntshengn_script_manager_interface.h"
#include "../scripting/ntshengn_scripting.h"
#include <string>

namespace NtshEngn {

	class SceneManager : public SceneManagerInterface {
	public:
		void goToScene(const std::string& filePath);

		std::string getCurrentScenePath();

		void createEntitiesFromScene(const std::string& filePath);

	public:
		void setECS(ECS* ecs);

		void setAssetManager(AssetManager* assetManager);

		void setScriptManager(ScriptManagerInterface* scriptManager);

		void setScripting(Scripting* scripting);

	private:
		AssetManager* m_assetManager;

		ECS* m_ecs;

		ScriptManagerInterface* m_scriptManager;

		Scripting* m_scripting;

		std::string m_currentScenePath = "";
	};

}
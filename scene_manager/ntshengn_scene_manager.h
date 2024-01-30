#pragma once
#include "../Common/asset_manager/ntshengn_asset_manager.h"
#include "../Common/ecs/ntshengn_ecs.h"
#include "../scripting/ntshengn_script_keeper.h"
#include <string>

namespace NtshEngn {

	class SceneManager {
	public:
		void goToScene(const std::string& filePath);

		std::string getCurrentScenePath();

		void setECS(ECS* ecs);
		void setAssetManager(AssetManager* assetManager);
		void setScriptKeeper(ScriptKeeper* scriptKeeper);

	private:
		AssetManager* m_assetManager;

		ECS* m_ecs;

		ScriptKeeper* m_scriptKeeper;

		std::string m_currentScenePath = "";
	};

}
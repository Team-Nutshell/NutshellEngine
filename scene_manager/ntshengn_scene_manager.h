#pragma once
#include "../Common/asset_manager/ntshengn_asset_manager.h"
#include "../Common/ecs/ntshengn_ecs.h"
#include <string>

namespace NtshEngn {

	class SceneManager {
	public:
		void goToScene(const std::string& filePath);

		std::string getCurrentScenePath();

		void setECS(ECS* ecs);
		void setAssetManager(AssetManager* assetManager);

	private:
		AssetManager* m_assetManager;

		ECS* m_ecs;

		std::string m_currentScenePath = "";
	};

}
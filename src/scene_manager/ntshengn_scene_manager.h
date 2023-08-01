#pragma once
#include "../../Common/asset_manager/ntshengn_asset_manager.h"
#include "../../Common/ecs/ntshengn_ecs.h"
#include <string>

namespace NtshEngn {

	class SceneManager {
	public:
		void goToScene(const std::string& filePath);

		void setAssetManager(AssetManager* assetManager);
		void setECS(ECS* ecs);

	private:
		AssetManager* m_assetManager;

		ECS* m_ecs;
	};

}
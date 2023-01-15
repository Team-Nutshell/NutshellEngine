#pragma once
#include "../../external/Common/resources/ntshengn_resources_audio.h"
#include "../../external/Common/resources/ntshengn_resources_graphics.h"
#include <string>
#include <forward_list>

namespace NtshEngn {

	class AssetLoader {
	public:
		Sound* loadSound(const std::string& filePath);
		Model* loadModel(const std::string& filePath);

		void unloadSound(Sound* sound);
		void unloadModel(Model* model);

	private:
		void loadSoundWav(const std::string& filePath, Sound& sound);
		void loadModelObj(const std::string& filePath, Model& model);

		std::forward_list<Sound> m_soundResources;
		std::forward_list<Model> m_modelResources;
	};

}
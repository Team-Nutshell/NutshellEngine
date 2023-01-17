#pragma once
#include "../../external/Common/resources/ntshengn_resources_audio.h"
#include "../../external/Common/resources/ntshengn_resources_graphics.h"
#include <string>
#include <forward_list>

namespace NtshEngn {

	class AssetManager {
	public:
		Sound* createSound();
		Sound* loadSound(const std::string& filePath);

		Model* createModel();
		Model* loadModel(const std::string& filePath);

		Image* createImage();
		Image* loadImage(const std::string& filePath);

		void destroySound(Sound* sound);
		void destroyModel(Model* model);
		void destroyImage(Image* image);

	private:
		void loadSoundWav(const std::string& filePath, Sound& sound);
		void loadModelObj(const std::string& filePath, Model& model);

		std::forward_list<Sound> m_soundResources;
		std::forward_list<Model> m_modelResources;
		std::forward_list<Image> m_imageResources;
	};

}
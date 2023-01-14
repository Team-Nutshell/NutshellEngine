#pragma once
#include "../../external/Common/resources/ntsh_resources_audio.h"
#include "../../external/Common/resources/ntsh_resources_graphics.h"
#include <string>
#include <forward_list>

class AssetLoader {
public:
	Ntsh::Sound* loadSound(const std::string& filePath);
	Ntsh::Model* loadModel(const std::string& filePath);

	void unloadSound(Ntsh::Sound* sound);
	void unloadModel(Ntsh::Model* model);

private:
	void loadSoundWav(const std::string& filePath, Ntsh::Sound& sound);
	void loadModelObj(const std::string& filePath, Ntsh::Model& model);

	std::forward_list<Ntsh::Sound> m_soundResources;
	std::forward_list<Ntsh::Model> m_modelResources;
};
#pragma once
#include "../../external/Common/resources/ntsh_resources_audio.h"
#include "../../external/Common/resources/ntsh_resources_graphics.h"
#include <string>
#include <forward_list>

class AssetLoader {
public:
	NtshAudio* loadAudio(const std::string& filePath);
	NtshModel* loadModel(const std::string& filePath);

	void unloadAudio(NtshAudio* audio);
	void unloadModel(NtshModel* model);

private:
	void loadAudioWav(const std::string& filePath, NtshAudio& audio);
	void loadModelObj(const std::string& filePath, NtshModel& model);

	std::forward_list<NtshAudio> m_audioResources;
	std::forward_list<NtshModel> m_modelResources;
};
#pragma once
#include "../../external/Common/resources/ntsh_resources_audio.h"
#include "../../external/Common/resources/ntsh_resources_graphics.h"
#include <string>

class AssetLoader {
public:
	NtshAudio loadAudio(const std::string& filePath);

private:
	void loadAudioWav(const std::string& filePath, NtshAudio& audio);
};
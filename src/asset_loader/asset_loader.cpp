#include "asset_loader.h"
#include "../../external/Common/utils/ntsh_engine_defines.h"
#include "../../external/Common/utils/ntsh_utils_file.h"
#include "../utils/ntsh_core_defines.h"
#include <iterator>

NtshAudio AssetLoader::loadAudio(const std::string& filePath) {
	NtshAudio newAudio;

	if (utils::file::extension(filePath) == "wav") {
		loadAudioWav(filePath, newAudio);
	}
	else {
		NTSH_ASSET_LOADER_ERROR("Audio file extension \"." + utils::file::extension(filePath) + "\" not supported.", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	return newAudio;
}

void AssetLoader::loadAudioWav(const std::string& filePath, NtshAudio& audio) {
	char buffer[4];
	int64_t tmp = 0;
	std::vector<char> data;

	std::ifstream file(filePath, std::ios::binary);

	// Open file
	if (!file.is_open()) {
		NTSH_ASSET_LOADER_ERROR("Could not open audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_FILE_NOT_FOUND);
	}

	// RIFF header
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read \"RIFF\" for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	if (strncmp(buffer, "RIFF", 4) != 0) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (RIFF header missing).", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// Size
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read size for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// WAVE header
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read \"WAVE\" for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	if (strncmp(buffer, "WAVE", 4) != 0) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (WAVE header missing).", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// fmt/0
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read fmt/0 for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// 16
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read 16 for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// PCM
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read PCM for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// Channels
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read the number of channels for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	memcpy(&tmp, buffer, 2);
	audio.channels = static_cast<uint8_t>(tmp);

	// Sample rate
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read sample rate for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	memcpy(&tmp, buffer, 4);
	audio.sampleRate = static_cast<int32_t>(tmp);

	// Byte rate ((sampleRate * bitsPerSample * channels) / 8)
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read byte rate for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// Block align ((bitsPerSample * channels) / 8)
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read block align for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// Bits per sample
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read bits per sample for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	memcpy(&tmp, buffer, 2);
	audio.bitsPerSample = static_cast<uint8_t>(tmp);

	// data header
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read \"data\" for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	if (strncmp(buffer, "data", 4) != 0) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (data header missing).", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// Data size
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read data size for audio file \"" + filePath + "\".", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	memcpy(&tmp, buffer, 4);
	audio.size = static_cast<size_t>(tmp);

	if (file.eof()) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (data missing).", NTSH_RESULT_ASSET_LOADER_ERROR);
	}
	if (file.fail()) {
		NTSH_ASSET_LOADER_ERROR("Unknown error while loading WAVE file.", NTSH_RESULT_ASSET_LOADER_ERROR);
	}

	// Data
	data.resize(audio.size);
	file.read(&data[0], audio.size);
	audio.data.insert(audio.data.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	data.erase(data.begin(), data.end());
}

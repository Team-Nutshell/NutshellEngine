#include "asset_loader.h"
#include "../../external/Common/utils/ntsh_engine_defines.h"
#include "../../external/Common/utils/ntsh_utils_file.h"
#include "../utils/ntsh_core_defines.h"
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <array>

Ntsh::Sound* AssetLoader::loadSound(const std::string& filePath) {
	Ntsh::Sound newSound;

	if (Ntsh::Utils::File::extension(filePath) == "wav") {
		loadSoundWav(filePath, newSound);
	}
	else {
		NTSH_ASSET_LOADER_ERROR("Audio file extension \"." + Ntsh::Utils::File::extension(filePath) + "\" not supported.", Ntsh::Result::AssetLoaderError);
	}

	m_soundResources.push_front(newSound);
	return &m_soundResources.front();
}

Ntsh::Model* AssetLoader::loadModel(const std::string& filePath) {
	Ntsh::Model newModel;

	if (Ntsh::Utils::File::extension(filePath) == "obj") {
		loadModelObj(filePath, newModel);
	}
	else {
		NTSH_ASSET_LOADER_ERROR("Model file extension \"." + Ntsh::Utils::File::extension(filePath) + "\" not supported.", Ntsh::Result::AssetLoaderError);
	}

	m_modelResources.push_front(newModel);
	return &m_modelResources.front();
}

void AssetLoader::unloadSound(Ntsh::Sound* audio) {
	std::forward_list<Ntsh::Sound>::iterator prev = m_soundResources.before_begin();
	for (std::forward_list<Ntsh::Sound>::iterator it = m_soundResources.begin(); it != m_soundResources.end(); it++) {
		if (audio == &(*it)) {
			m_soundResources.erase_after(prev);
			break;
		}

		prev = it;
	}

	NTSH_ASSET_LOADER_ERROR("Could not unload audio resource.", Ntsh::Result::AssetLoaderError);
}

void AssetLoader::unloadModel(Ntsh::Model* model) {
	std::forward_list<Ntsh::Model>::iterator prev = m_modelResources.before_begin();
	for (std::forward_list<Ntsh::Model>::iterator it = m_modelResources.begin(); it != m_modelResources.end(); it++) {
		if (model == &(*it)) {
			m_modelResources.erase_after(prev);
			return;
		}

		prev = it;
	}

	NTSH_ASSET_LOADER_ERROR("Could not unload model resource.", Ntsh::Result::AssetLoaderError);
}

void AssetLoader::loadSoundWav(const std::string& filePath, Ntsh::Sound& sound) {
	char buffer[4];
	int64_t tmp = 0;
	std::vector<char> data;

	std::ifstream file(filePath, std::ios::binary);

	// Open file
	if (!file.is_open()) {
		NTSH_ASSET_LOADER_ERROR("Could not open audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderFileNotFound);
	}

	// RIFF header
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read \"RIFF\" for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}
	if (strncmp(buffer, "RIFF", 4) != 0) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (RIFF header missing).", Ntsh::Result::AssetLoaderError);
	}

	// Size
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read size for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}

	// WAVE header
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read \"WAVE\" for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}
	if (strncmp(buffer, "WAVE", 4) != 0) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (WAVE header missing).", Ntsh::Result::AssetLoaderError);
	}

	// fmt/0
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read fmt/0 for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}

	// 16
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read 16 for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}

	// PCM
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read PCM for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}

	// Channels
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read the number of channels for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}
	memcpy(&tmp, buffer, 2);
	sound.channels = static_cast<uint8_t>(tmp);

	// Sample rate
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read sample rate for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}
	memcpy(&tmp, buffer, 4);
	sound.sampleRate = static_cast<int32_t>(tmp);

	// Byte rate ((sampleRate * bitsPerSample * channels) / 8)
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read byte rate for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}

	// Block align ((bitsPerSample * channels) / 8)
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read block align for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}

	// Bits per sample
	if (!file.read(buffer, 2)) {
		NTSH_ASSET_LOADER_ERROR("Could not read bits per sample for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}
	memcpy(&tmp, buffer, 2);
	sound.bitsPerSample = static_cast<uint8_t>(tmp);

	// data header
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read \"data\" for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}
	if (strncmp(buffer, "data", 4) != 0) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (data header missing).", Ntsh::Result::AssetLoaderError);
	}

	// Data size
	if (!file.read(buffer, 4)) {
		NTSH_ASSET_LOADER_ERROR("Could not read data size for audio file \"" + filePath + "\".", Ntsh::Result::AssetLoaderError);
	}
	memcpy(&tmp, buffer, 4);
	sound.size = static_cast<size_t>(tmp);

	if (file.eof()) {
		NTSH_ASSET_LOADER_ERROR("File \"" + filePath + "\" is not a valid WAVE audio file (data missing).", Ntsh::Result::AssetLoaderError);
	}
	if (file.fail()) {
		NTSH_ASSET_LOADER_ERROR("Unknown error while loading WAVE file.", Ntsh::Result::AssetLoaderError);
	}

	// Data
	data.resize(sound.size);
	file.read(&data[0], sound.size);
	sound.data.insert(sound.data.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	data.erase(data.begin(), data.end());

	file.close();
}

void AssetLoader::loadModelObj(const std::string& filePath, Ntsh::Model& model) {
	NTSH_UNUSED(model);
	std::ifstream file(filePath);

	// Open file
	if (!file.is_open()) {
		NTSH_ASSET_LOADER_ERROR("Could not open model file \"" + filePath + "\".", Ntsh::Result::AssetLoaderFileNotFound);
	}

	std::vector<std::array<float, 3>> positions;
	std::vector<std::array<float, 3>> normals;
	std::vector<std::array<float, 2>> uvs;

	std::unordered_map<std::string, uint32_t> uniqueVertices;
	Ntsh::Mesh mesh = {};
	mesh.topology = Ntsh::MeshTopology::TriangleList;

	std::string line;
	while (std::getline(file, line)) {
		// Ignore comment
		if (line[0] == '#') {
			continue;
		}

		// Parse line
		std::vector<std::string> tokens;
		size_t spacePosition = 0;
		while ((spacePosition = line.find(' ')) != std::string::npos) {
			tokens.push_back(line.substr(0, spacePosition));
			line.erase(0, spacePosition + 1);
		}
		tokens.push_back(line);

		// Parse tokens
		// Position
		if (tokens[0] == "v") {
			positions.push_back({
				static_cast<float>(std::atof(tokens[1].c_str())),
				static_cast<float>(std::atof(tokens[2].c_str())),
				static_cast<float>(std::atof(tokens[3].c_str()))
				});
		}
		// Normal
		else if (tokens[0] == "vn") {
			normals.push_back({
				static_cast<float>(std::atof(tokens[1].c_str())),
				static_cast<float>(std::atof(tokens[2].c_str())),
				static_cast<float>(std::atof(tokens[3].c_str()))
				});
		}
		// UV
		else if (tokens[0] == "vt") {
			uvs.push_back({
				static_cast<float>(std::atof(tokens[1].c_str())),
				static_cast<float>(std::atof(tokens[2].c_str()))
				});
		}
		// Face
		else if (tokens[0] == "f") {
			std::vector<uint32_t> tmpIndices;
			for (size_t i = 1; i < tokens.size(); i++) {
				Ntsh::Vertex vertex = {};

				std::string tmp = tokens[i];
				std::vector<std::string> valueIndices;
				size_t slashPosition = 0;
				while ((slashPosition = tmp.find('/')) != std::string::npos) {
					valueIndices.push_back(tmp.substr(0, slashPosition));
					tmp.erase(0, slashPosition + 1);
				}
				valueIndices.push_back(tmp);

				for (size_t j = 0; j < valueIndices.size(); j++) {
					if (valueIndices[j] != "") {
						// v/vt/vn
						// Position index
						if (j == 0) {
							vertex.position[0] = positions[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][0];
							vertex.position[1] = positions[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][1];
							vertex.position[2] = positions[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][2];
						}
						// UV index
						else if (j == 1) {
							vertex.uv[0] = uvs[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][0];
							vertex.uv[1] = uvs[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][1];
						}
						// Normal index
						else if (j == 2) {
							vertex.normal[0] = normals[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][0];
							vertex.normal[1] = normals[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][1];
							vertex.normal[2] = normals[static_cast<size_t>(std::atoi(valueIndices[j].c_str())) - 1][2];
						}
					}
				}

				if (uniqueVertices.count(tokens[i]) == 0) {
					uniqueVertices[tokens[i]] = static_cast<uint32_t>(mesh.vertices.size());
					mesh.vertices.push_back(vertex);
				}
				tmpIndices.push_back(uniqueVertices[tokens[i]]);
			}

			// Face can be a triangle or a rectangle
			// Triangle
			if (tmpIndices.size() == 3) {
				mesh.indices.insert(mesh.indices.end(), std::make_move_iterator(tmpIndices.begin()), std::make_move_iterator(tmpIndices.end()));
			}
			// Rectangle
			else if (tmpIndices.size() == 4) {
				// Triangle 1
				mesh.indices.push_back(tmpIndices[0]);
				mesh.indices.push_back(tmpIndices[1]);
				mesh.indices.push_back(tmpIndices[2]);

				// Triangle 2
				mesh.indices.push_back(tmpIndices[0]);
				mesh.indices.push_back(tmpIndices[2]);
				mesh.indices.push_back(tmpIndices[3]);
			}
		}
	}

	model.primitives.push_back({ mesh, {} });

	file.close();
}

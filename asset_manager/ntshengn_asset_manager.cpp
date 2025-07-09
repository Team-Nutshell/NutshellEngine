#include "ntshengn_asset_manager.h"
#include "../utils/ntshengn_defines.h"
#include "../utils/ntshengn_utils_bimap.h"
#include "../utils/ntshengn_utils_file.h"
#include "../utils/ntshengn_utils_json.h"
#include "../utils/ntshengn_utils_math.h"
#include <string>
#include <iterator>
#include <algorithm>
#include <array>
#include <utility>
#include <numeric>
#include <cmath>

NtshEngn::Model* NtshEngn::AssetManager::createModel(const std::string& modelName) {
	if (m_models.find(modelName) == m_models.end()) {
		Model newModel;
		m_models[modelName] = newModel;
		Model* model = &m_models[modelName];

		m_modelNames[model] = modelName;

		return model;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Model name \"" + modelName + "\" already exists.");

		return nullptr;
	}
}

NtshEngn::Model* NtshEngn::AssetManager::loadModel(const std::string& filePath) {
	if (!std::filesystem::exists(filePath)) {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load model file \"" + filePath + "\" (file does not exist).");

		return nullptr;
	}

	std::string normalizedPath = getNormalizedPath(filePath);

	if (m_models.find(normalizedPath) != m_models.end()) {
		if (m_modelLastModified.find(normalizedPath) != m_modelLastModified.end()) {
			if (m_modelLastModified[normalizedPath] == std::filesystem::last_write_time(filePath)) {
				return &m_models[normalizedPath];
			}
		}
	}

	Model newModel;
	if (File::extension(filePath) == "ntmd") {
		loadModelNtmd(filePath, newModel);
	}
	else {
		if (m_assetLoaderModule) {
			newModel = m_assetLoaderModule->loadModel(filePath);
		}
	}

	if (newModel.primitives.size() != 0) {
		m_models[normalizedPath] = newModel;
		Model* model = &m_models[normalizedPath];

		m_modelNames[model] = normalizedPath;
		m_modelLastModified[normalizedPath] = std::filesystem::last_write_time(filePath);

		return model;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load model file \"" + filePath + "\".");

		return nullptr;
	}
}

NtshEngn::Material* NtshEngn::AssetManager::createMaterial(const std::string& materialName) {
	if (m_materials.find(materialName) == m_materials.end()) {
		Material newMaterial;
		m_materials[materialName] = newMaterial;
		Material* material = &m_materials[materialName];

		m_materialNames[material] = materialName;

		return material;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Material name \"" + materialName + "\" already exists.");

		return nullptr;
	}
}

NtshEngn::Material* NtshEngn::AssetManager::loadMaterial(const std::string& filePath) {
	if (!std::filesystem::exists(filePath)) {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load material file \"" + filePath + "\" (file does not exist).");

		return nullptr;
	}

	std::string normalizedPath = getNormalizedPath(filePath);

	if (m_materials.find(normalizedPath) != m_materials.end()) {
		if (m_materialLastModified.find(normalizedPath) != m_materialLastModified.end()) {
			if (m_materialLastModified[normalizedPath] == std::filesystem::last_write_time(filePath)) {
				return &m_materials[normalizedPath];
			}
		}
	}

	Material newMaterial;
	if (File::extension(filePath) == "ntml") {
		loadMaterialNtml(filePath, newMaterial);
	}
	else {
		if (m_assetLoaderModule) {
			newMaterial = m_assetLoaderModule->loadMaterial(filePath);
		}
	}

	m_materials[normalizedPath] = newMaterial;
	Material* material = &m_materials[normalizedPath];

	m_materialNames[material] = normalizedPath;
	m_materialLastModified[normalizedPath] = std::filesystem::last_write_time(filePath);

	return material;
}

NtshEngn::Image* NtshEngn::AssetManager::createImage(const std::string& imageName) {
	if (m_images.find(imageName) == m_images.end()) {
		Image newImage;
		m_images[imageName] = newImage;
		Image* image = &m_images[imageName];

		m_imageNames[image] = imageName;

		return image;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Image name \"" + imageName + "\" already exists.");

		return nullptr;
	}
}

NtshEngn::Image* NtshEngn::AssetManager::loadImage(const std::string& filePath) {
	if (!std::filesystem::exists(filePath)) {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load image file \"" + filePath + "\" (file does not exist).");

		return nullptr;
	}

	std::string normalizedPath = getNormalizedPath(filePath);

	if (m_images.find(normalizedPath) != m_images.end()) {
		if (m_imageLastModified.find(normalizedPath) != m_imageLastModified.end()) {
			if (m_imageLastModified[normalizedPath] == std::filesystem::last_write_time(filePath)) {
				return &m_images[normalizedPath];
			}
		}
	}

	Image newImage;
	if (File::extension(filePath) == "ntim") {
		loadImageNtim(filePath, newImage);
	}
	else {
		if (m_assetLoaderModule) {
			newImage = m_assetLoaderModule->loadImage(filePath);
		}
	}

	if (newImage.width != 0) {
		m_images[normalizedPath] = newImage;
		Image* image = &m_images[normalizedPath];

		m_imageNames[image] = normalizedPath;
		m_imageLastModified[normalizedPath] = std::filesystem::last_write_time(filePath);

		return image;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load image file \"" + filePath + "\".");

		return nullptr;
	}
}

NtshEngn::Font* NtshEngn::AssetManager::createFont(const std::string& fontName) {
	if (m_fonts.find(fontName) == m_fonts.end()) {
		Font newFont;
		m_fonts[fontName] = newFont;
		Font* font = &m_fonts[fontName];

		m_fontNames[font] = fontName;

		return font;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Image name \"" + fontName + "\" already exists.");

		return nullptr;
	}
}

NtshEngn::Font* NtshEngn::AssetManager::loadFontBitmap(const std::string& filePath, float fontHeight) {
	if (!std::filesystem::exists(filePath)) {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load font file \"" + filePath + "\" (file does not exist).");

		return nullptr;
	}

	if (fontHeight <= 0.0f) {
		NTSHENGN_ASSET_MANAGER_WARNING("Font height for font file \"" + filePath + "\" should be greater than 0.");

		return nullptr;
	}

	std::string normalizedPath = getNormalizedPath(filePath) + ":Bitmap:" + std::to_string(fontHeight);

	if (m_fonts.find(normalizedPath) != m_fonts.end()) {
		if (m_fontLastModified.find(normalizedPath) != m_fontLastModified.end()) {
			if (m_fontLastModified[normalizedPath] == std::filesystem::last_write_time(filePath)) {
				return &m_fonts[normalizedPath];
			}
		}
	}

	Font newFont;
	if (m_assetLoaderModule) {
		newFont = m_assetLoaderModule->loadFontBitmap(filePath, fontHeight);
	}

	if (newFont.image) {
		newFont.type = FontType::Bitmap;

		m_fonts[normalizedPath] = newFont;
		Font* font = &m_fonts[normalizedPath];

		m_fontNames[font] = normalizedPath;
		m_fontLastModified[normalizedPath] = std::filesystem::last_write_time(filePath);

		return font;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load font file \"" + filePath + "\".");

		return nullptr;
	}
}

NtshEngn::Font* NtshEngn::AssetManager::loadFontSDF(const std::string& filePath) {
	if (!std::filesystem::exists(filePath)) {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load font file \"" + filePath + "\" (file does not exist).");

		return nullptr;
	}

	std::string normalizedPath = getNormalizedPath(filePath) + ":SDF";

	if (m_fonts.find(normalizedPath) != m_fonts.end()) {
		if (m_fontLastModified.find(normalizedPath) != m_fontLastModified.end()) {
			if (m_fontLastModified[normalizedPath] == std::filesystem::last_write_time(filePath)) {
				return &m_fonts[normalizedPath];
			}
		}
	}

	Font newFont;
	if (m_assetLoaderModule) {
		newFont = m_assetLoaderModule->loadFontSDF(filePath);
	}

	if (newFont.image) {
		newFont.type = FontType::SDF;

		m_fonts[normalizedPath] = newFont;
		Font* font = &m_fonts[normalizedPath];

		m_fontNames[font] = normalizedPath;
		m_fontLastModified[normalizedPath] = std::filesystem::last_write_time(filePath);

		return font;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load font file \"" + filePath + "\".");

		return nullptr;
	}
}

NtshEngn::Sound* NtshEngn::AssetManager::createSound(const std::string& soundName) {
	if (m_sounds.find(soundName) == m_sounds.end()) {
		Sound newSound;
		m_sounds[soundName] = newSound;
		Sound* sound = &m_sounds[soundName];

		m_soundNames[sound] = soundName;

		return sound;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Sound name \"" + soundName + "\" already exists.");

		return nullptr;
	}
}

NtshEngn::Sound* NtshEngn::AssetManager::loadSound(const std::string& filePath) {
	if (!std::filesystem::exists(filePath)) {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load sound file \"" + filePath + "\" (file does not exist).");

		return nullptr;
	}

	std::string normalizedPath = getNormalizedPath(filePath);

	if (m_sounds.find(normalizedPath) != m_sounds.end()) {
		if (m_soundLastModified.find(normalizedPath) != m_soundLastModified.end()) {
			if (m_soundLastModified[normalizedPath] == std::filesystem::last_write_time(filePath)) {
				return &m_sounds[normalizedPath];
			}
		}
	}

	Sound newSound;
	if (File::extension(filePath) == "ntsd") {
		loadSoundNtsd(filePath, newSound);
	}
	else {
		if (m_assetLoaderModule) {
			newSound = m_assetLoaderModule->loadSound(filePath);
		}
	}

	if (newSound.size != 0) {
		m_sounds[normalizedPath] = newSound;
		Sound* sound = &m_sounds[normalizedPath];

		m_soundNames[sound] = normalizedPath;
		m_soundLastModified[normalizedPath] = std::filesystem::last_write_time(filePath);

		return sound;
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not load sound file \"" + filePath + "\".");

		return nullptr;
	}
}

void NtshEngn::AssetManager::destroyModel(const std::string& modelName) {
	if (m_models.find(modelName) != m_models.end()) {
		m_modelNames.erase(&m_models[modelName]);
		if (m_modelLastModified.find(modelName) != m_modelLastModified.end()) {
			m_modelLastModified.erase(modelName);
		}
		m_models.erase(modelName);
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not destroy model named \"" + modelName + "\" (name not found).");
	}
}

void NtshEngn::AssetManager::destroyMaterial(const std::string& materialName) {
	if (m_materials.find(materialName) != m_materials.end()) {
		m_materialNames.erase(&m_materials[materialName]);
		if (m_materialLastModified.find(materialName) != m_materialLastModified.end()) {
			m_materialLastModified.erase(materialName);
		}
		m_materials.erase(materialName);
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not destroy material named \"" + materialName + "\" (name not found).");
	}
}

void NtshEngn::AssetManager::destroyImage(const std::string& imageName) {
	if (m_images.find(imageName) != m_images.end()) {
		m_imageNames.erase(&m_images[imageName]);
		if (m_imageLastModified.find(imageName) != m_imageLastModified.end()) {
			m_imageLastModified.erase(imageName);
		}
		m_images.erase(imageName);
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not destroy image named \"" + imageName + "\" (name not found).");
	}
}

void NtshEngn::AssetManager::destroyFont(const std::string& fontName) {
	if (m_fonts.find(fontName) != m_fonts.end()) {
		m_fontNames.erase(&m_fonts[fontName]);
		if (m_fontLastModified.find(fontName) != m_fontLastModified.end()) {
			m_fontLastModified.erase(fontName);
		}
		m_fonts.erase(fontName);
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not destroy font named \"" + fontName + "\" (name not found).");
	}
}

void NtshEngn::AssetManager::destroySound(const std::string& soundName) {
	if (m_sounds.find(soundName) != m_sounds.end()) {
		m_soundNames.erase(&m_sounds[soundName]);
		if (m_soundLastModified.find(soundName) != m_soundLastModified.end()) {
			m_soundLastModified.erase(soundName);
		}
		m_sounds.erase(soundName);
	}
	else {
		NTSHENGN_ASSET_MANAGER_WARNING("Could not destroy sound named \"" + soundName + "\" (name not found).");
	}
}

NtshEngn::Model* NtshEngn::AssetManager::findModelByName(const std::string& modelName) {
	if (m_models.find(modelName) != m_models.end()) {
		return &m_models[modelName];
	}
	else {
		return nullptr;
	}
}

std::string NtshEngn::AssetManager::getModelName(const Model* model) {
	if (m_modelNames.find(const_cast<Model*>(model)) != m_modelNames.end()) {
		return m_modelNames[const_cast<Model*>(model)];
	}
	else {
		return "";
	}
}

NtshEngn::Material* NtshEngn::AssetManager::findMaterialByName(const std::string& materialName) {
	if (m_materials.find(materialName) != m_materials.end()) {
		return &m_materials[materialName];
	}
	else {
		return nullptr;
	}
}

std::string NtshEngn::AssetManager::getMaterialName(const Material* material) {
	if (m_materialNames.find(const_cast<Material*>(material)) != m_materialNames.end()) {
		return m_materialNames[const_cast<Material*>(material)];
	}
	else {
		return "";
	}
}

NtshEngn::Image* NtshEngn::AssetManager::findImageByName(const std::string& imageName) {
	if (m_images.find(imageName) != m_images.end()) {
		return &m_images[imageName];
	}
	else {
		return nullptr;
	}
}

std::string NtshEngn::AssetManager::getImageName(const Image* image) {
	if (m_imageNames.find(const_cast<Image*>(image)) != m_imageNames.end()) {
		return m_imageNames[const_cast<Image*>(image)];
	}
	else {
		return "";
	}
}

NtshEngn::Font* NtshEngn::AssetManager::findFontByName(const std::string& fontName) {
	if (m_fonts.find(fontName) != m_fonts.end()) {
		return &m_fonts[fontName];
	}
	else {
		return nullptr;
	}
}

std::string NtshEngn::AssetManager::getFontName(const Font* font) {
	if (m_fontNames.find(const_cast<Font*>(font)) != m_fontNames.end()) {
		return m_fontNames[const_cast<Font*>(font)];
	}
	else {
		return "";
	}
}

NtshEngn::Sound* NtshEngn::AssetManager::findSoundByName(const std::string& soundName) {
	if (m_sounds.find(soundName) != m_sounds.end()) {
		return &m_sounds[soundName];
	}
	else {
		return nullptr;
	}
}

std::string NtshEngn::AssetManager::getSoundName(const Sound* sound) {
	if (m_soundNames.find(const_cast<Sound*>(sound)) != m_soundNames.end()) {
		return m_soundNames[const_cast<Sound*>(sound)];
	}
	else {
		return "";
	}
}

void NtshEngn::AssetManager::calculateTangents(Mesh& mesh) {
	std::vector<Math::vec3> tan1(mesh.vertices.size());
	std::vector<Math::vec3> tan2(mesh.vertices.size());
	for (size_t i = 0; i < mesh.indices.size(); i += 3) {
		const NtshEngn::Vertex& vertex0 = mesh.vertices[mesh.indices[i]];
		const NtshEngn::Vertex& vertex1 = mesh.vertices[mesh.indices[i + 1]];
		const NtshEngn::Vertex& vertex2 = mesh.vertices[mesh.indices[i + 2]];

		const Math::vec3 dPos1 = vertex1.position - vertex0.position;
		const Math::vec3 dPos2 = vertex2.position - vertex0.position;

		const Math::vec2 dUV1 = vertex1.uv - vertex0.uv;
		const Math::vec2 dUV2 = vertex2.uv - vertex0.uv;

		const float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

		const Math::vec3 uDir = (dPos1 * dUV2.y - dPos2 * dUV1.y) * r;
		const Math::vec3 vDir = (dPos2 * dUV1.x - dPos1 * dUV2.x) * r;

		tan1[mesh.indices[i]] += uDir;
		tan1[mesh.indices[i + 1]] += uDir;
		tan1[mesh.indices[i + 2]] += uDir;

		tan2[mesh.indices[i]] += vDir;
		tan2[mesh.indices[i + 1]] += vDir;
		tan2[mesh.indices[i + 2]] += vDir;
	}

	for (size_t i = 0; i < mesh.vertices.size(); i++) {
		const Math::vec3 n = mesh.vertices[i].normal;
		const Math::vec3 t = tan1[i];

		const Math::vec3 tangent = Math::normalize(t - n * Math::dot(n, t));
		const float tangentHandedness = (Math::dot(Math::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

		mesh.vertices[i].tangent = { tangent.x, tangent.y, tangent.z, tangentHandedness };
	}
}

std::array<NtshEngn::Math::vec3, 2> NtshEngn::AssetManager::calculateAABB(const Mesh& mesh) {
	Math::vec3 min = Math::vec3(std::numeric_limits<float>::max());
	Math::vec3 max = Math::vec3(std::numeric_limits<float>::lowest());
	for (const NtshEngn::Vertex& vertex : mesh.vertices) {
		if (vertex.position[0] < min.x) {
			min.x = vertex.position[0];
		}
		if (vertex.position[0] > max.x) {
			max.x = vertex.position[0];
		}

		if (vertex.position[1] < min.y) {
			min.y = vertex.position[1];
		}
		if (vertex.position[1] > max.y) {
			max.y = vertex.position[1];
		}

		if (vertex.position[2] < min.z) {
			min.z = vertex.position[2];
		}
		if (vertex.position[2] > max.z) {
			max.z = vertex.position[2];
		}
	}

	const float epsilon = 0.0001f;

	if (min.x == max.x) {
		min.x -= epsilon;
		max.x += epsilon;
	}

	if (min.y == max.y) {
		min.y -= epsilon;
		max.y += epsilon;
	}

	if (min.z == max.z) {
		min.z -= epsilon;
		max.z += epsilon;
	}

	return { min, max };
}

void NtshEngn::AssetManager::setAssetLoaderModule(AssetLoaderModuleInterface* assetLoaderModule) {
	m_assetLoaderModule = assetLoaderModule;
}

void NtshEngn::AssetManager::loadMeshNtmh(const std::string& filePath, Mesh& mesh) {
	const std::unordered_map<std::string, MeshTopology> stringToMeshTopology {
		{ "TriangleList", MeshTopology::TriangleList },
		{ "TriangleStrip", MeshTopology::TriangleStrip },
		{ "LineList", MeshTopology::LineList },
		{ "LineStrip", MeshTopology::LineStrip },
		{ "PointList", MeshTopology::PointList },
		{ "Unknown", MeshTopology::Unknown }
	};

	JSON json;
	const JSON::Node& meshRoot = json.read(filePath);

	bool hasNormals = false;
	bool hasUvs = false;
	bool hasTangents = false;

	if (meshRoot.contains("vertices")) {
		for (size_t i = 0; i < meshRoot["vertices"].size(); i++) {
			const JSON::Node& vertexNode = meshRoot["vertices"][i];

			Vertex vertex;
			if (vertexNode.contains("position")) {
				vertex.position = { vertexNode["position"][0].getNumber(), vertexNode["position"][1].getNumber(), vertexNode["position"][2].getNumber() };
			}

			if (vertexNode.contains("normal")) {
				vertex.normal = { vertexNode["normal"][0].getNumber(), vertexNode["normal"][1].getNumber(), vertexNode["normal"][2].getNumber() };
				hasNormals = true;
			}

			if (vertexNode.contains("uv")) {
				vertex.uv = { vertexNode["uv"][0].getNumber(), vertexNode["uv"][1].getNumber() };
				hasUvs = true;
			}

			if (vertexNode.contains("color")) {
				vertex.color = { vertexNode["color"][0].getNumber(), vertexNode["color"][1].getNumber(), vertexNode["color"][2].getNumber() };
			}

			if (vertexNode.contains("tangent")) {
				vertex.tangent = { vertexNode["tangent"][0].getNumber(), vertexNode["tangent"][1].getNumber(), vertexNode["tangent"][2].getNumber(), vertexNode["tangent"][3].getNumber() };
				hasTangents = true;
			}

			if (vertexNode.contains("joints")) {
				vertex.joints = std::array<uint32_t, 4>({ static_cast<uint32_t>(vertexNode["joints"][0].getNumber()), static_cast<uint32_t>(vertexNode["joints"][1].getNumber()), static_cast<uint32_t>(vertexNode["joints"][2].getNumber()), static_cast<uint32_t>(vertexNode["joints"][3].getNumber()) });
			}

			if (vertexNode.contains("weights")) {
				vertex.weights = { vertexNode["weights"][0].getNumber(), vertexNode["weights"][1].getNumber(), vertexNode["weights"][2].getNumber(), vertexNode["weights"][3].getNumber() };
			}

			mesh.vertices.push_back(vertex);
		}
	}

	if (meshRoot.contains("indices")) {
		for (size_t i = 0; i < meshRoot["indices"].size(); i++) {
			mesh.indices.push_back(static_cast<uint32_t>(meshRoot["indices"][i].getNumber()));
		}
	}
	else {
		// Calculate indices
		mesh.indices.resize(mesh.vertices.size());
		std::iota(mesh.indices.begin(), mesh.indices.end(), 0);
	}

	// Calculate tangents
	if ((!hasTangents) && (hasNormals && hasUvs)) {
		calculateTangents(mesh);
	}

	if (meshRoot.contains("topology")) {
		mesh.topology = stringToMeshTopology.at(meshRoot["topology"].getString());
	}
}

void NtshEngn::AssetManager::loadImageSamplerNtsp(const std::string& filePath, ImageSampler& imageSampler) {
	const std::unordered_map<std::string, ImageSamplerFilter> stringToImageSamplerFilter{
		{ "Linear", ImageSamplerFilter::Linear },
		{ "Nearest", ImageSamplerFilter::Nearest },
		{ "Unknown", ImageSamplerFilter::Unknown }
	};
	const std::unordered_map<std::string, ImageSamplerAddressMode> stringToImageSamplerAddressMode{
		{ "Repeat", ImageSamplerAddressMode::Repeat },
		{ "MirroredRepeat", ImageSamplerAddressMode::MirroredRepeat },
		{ "ClampToEdge", ImageSamplerAddressMode::ClampToEdge },
		{ "ClampToBorder", ImageSamplerAddressMode::ClampToBorder },
		{ "Unknown", ImageSamplerAddressMode::Unknown }
	};
	const std::unordered_map<std::string, ImageSamplerBorderColor> stringToImageSamplerBorderColor{
		{ "FloatTransparentBlack", ImageSamplerBorderColor::FloatTransparentBlack },
		{ "IntTransparentBlack", ImageSamplerBorderColor::IntTransparentBlack },
		{ "FloatOpaqueBlack", ImageSamplerBorderColor::FloatOpaqueBlack },
		{ "IntOpaqueBlack", ImageSamplerBorderColor::IntOpaqueBlack },
		{ "FloatOpaqueWhite", ImageSamplerBorderColor::FloatOpaqueWhite },
		{ "IntOpaqueWhite", ImageSamplerBorderColor::IntOpaqueWhite },
		{ "Unknown", ImageSamplerBorderColor::Unknown }
	};

	JSON json;
	const JSON::Node& imageSamplerRoot = json.read(filePath);

	if (imageSamplerRoot.contains("magFilter")) {
		imageSampler.magFilter = stringToImageSamplerFilter.at(imageSamplerRoot["magFilter"].getString());
	}

	if (imageSamplerRoot.contains("minFilter")) {
		imageSampler.minFilter = stringToImageSamplerFilter.at(imageSamplerRoot["minFilter"].getString());
	}

	if (imageSamplerRoot.contains("mipmapFilter")) {
		imageSampler.mipmapFilter = stringToImageSamplerFilter.at(imageSamplerRoot["mipmapFilter"].getString());
	}

	if (imageSamplerRoot.contains("addressModeU")) {
		imageSampler.addressModeU = stringToImageSamplerAddressMode.at(imageSamplerRoot["addressModeU"].getString());
	}

	if (imageSamplerRoot.contains("addressModeV")) {
		imageSampler.addressModeV = stringToImageSamplerAddressMode.at(imageSamplerRoot["addressModeV"].getString());
	}

	if (imageSamplerRoot.contains("addressModeW")) {
		imageSampler.addressModeW = stringToImageSamplerAddressMode.at(imageSamplerRoot["addressModeW"].getString());
	}

	if (imageSamplerRoot.contains("borderColor")) {
		imageSampler.borderColor = stringToImageSamplerBorderColor.at(imageSamplerRoot["borderColor"].getString());
	}

	if (imageSamplerRoot.contains("anisotropyLevel")) {
		imageSampler.anisotropyLevel = imageSamplerRoot["anisotropyLevel"].getNumber();
	}
}

void NtshEngn::AssetManager::loadMaterialNtml(const std::string& filePath, Material& material) {
	JSON json;
	const JSON::Node& materialRoot = json.read(filePath);

	if (materialRoot.contains("diffuse")) {
		const JSON::Node& diffuseNode = materialRoot["diffuse"];

		if (diffuseNode.contains("texture")) {
			const JSON::Node& diffuseTextureNode = diffuseNode["texture"];

			if (diffuseTextureNode.contains("imagePath")) {
				material.diffuseTexture.image = loadImage(diffuseTextureNode["imagePath"].getString());
			}

			if (diffuseTextureNode.contains("imageSamplerPath")) {
				loadImageSamplerNtsp(diffuseTextureNode["imageSamplerPath"].getString(), material.diffuseTexture.imageSampler);
			}
		}
		else if (diffuseNode.contains("color")) {
			const JSON::Node& diffuseColorNode = diffuseNode["color"];

			std::string mapKey = "srgb " + std::to_string(diffuseColorNode[0].getNumber()) + " " + std::to_string(diffuseColorNode[1].getNumber()) + " " + std::to_string(diffuseColorNode[2].getNumber()) + " " + std::to_string(diffuseColorNode[3].getNumber());

			Image* image = findImageByName(mapKey);
			if (!image) {
				image = createImage(mapKey);
				image->width = 1;
				image->height = 1;
				image->format = ImageFormat::R8G8B8A8;
				image->colorSpace = ImageColorSpace::SRGB;
				image->data = { static_cast<uint8_t>(round(255.0f * diffuseColorNode[0].getNumber())), static_cast<uint8_t>(round(255.0f * diffuseColorNode[1].getNumber())), static_cast<uint8_t>(round(255.0f * diffuseColorNode[2].getNumber())), static_cast<uint8_t>(round(255.0f * diffuseColorNode[3].getNumber())) };
			}

			material.diffuseTexture.image = image;
		}
	}

	if (materialRoot.contains("normal")) {
		const JSON::Node& normalNode = materialRoot["normal"];

		if (normalNode.contains("texture")) {
			const JSON::Node& normalTextureNode = normalNode["texture"];

			if (normalTextureNode.contains("imagePath")) {
				material.normalTexture.image = loadImage(normalTextureNode["imagePath"].getString());
			}

			if (normalTextureNode.contains("imageSamplerPath")) {
				loadImageSamplerNtsp(normalTextureNode["imageSamplerPath"].getString(), material.normalTexture.imageSampler);
			}
		}
	}

	if (materialRoot.contains("metalness")) {
		const JSON::Node& metalnessNode = materialRoot["metalness"];

		if (metalnessNode.contains("texture")) {
			const JSON::Node& metalnessTextureNode = metalnessNode["texture"];

			if (metalnessTextureNode.contains("imagePath")) {
				material.metalnessTexture.image = loadImage(metalnessTextureNode["imagePath"].getString());
			}

			if (metalnessTextureNode.contains("imageSamplerPath")) {
				loadImageSamplerNtsp(metalnessTextureNode["imageSamplerPath"].getString(), material.metalnessTexture.imageSampler);
			}
		}
		else if (metalnessNode.contains("value")) {
			const JSON::Node& metalnessValueNode = metalnessNode["value"];
			uint8_t metalnessValue = static_cast<uint8_t>(round(255.0f * metalnessValueNode.getNumber()));

			std::string mapKey = "linear " + std::to_string(metalnessValueNode.getNumber()) + " " + std::to_string(metalnessValueNode.getNumber()) + " " + std::to_string(metalnessValueNode.getNumber()) + " " + std::to_string(metalnessValueNode.getNumber());

			Image* image = findImageByName(mapKey);
			if (!image) {
				image = createImage(mapKey);
				image->width = 1;
				image->height = 1;
				image->format = ImageFormat::R8G8B8A8;
				image->colorSpace = ImageColorSpace::Linear;
				image->data = { metalnessValue, metalnessValue, metalnessValue, metalnessValue };
			}

			material.metalnessTexture.image = image;
		}
	}

	if (materialRoot.contains("roughness")) {
		const JSON::Node& roughnessNode = materialRoot["roughness"];

		if (roughnessNode.contains("texture")) {
			const JSON::Node& roughnessTextureNode = roughnessNode["texture"];

			if (roughnessTextureNode.contains("imagePath")) {
				material.roughnessTexture.image = loadImage(roughnessTextureNode["imagePath"].getString());
			}

			if (roughnessTextureNode.contains("imageSamplerPath")) {
				loadImageSamplerNtsp(roughnessTextureNode["imageSamplerPath"].getString(), material.roughnessTexture.imageSampler);
			}
		}
		else if (roughnessNode.contains("value")) {
			const JSON::Node& roughnessValueNode = roughnessNode["value"];
			uint8_t roughnessValue = static_cast<uint8_t>(round(255.0f * roughnessValueNode.getNumber()));

			std::string mapKey = "linear " + std::to_string(roughnessValueNode.getNumber()) + " " + std::to_string(roughnessValueNode.getNumber()) + " " + std::to_string(roughnessValueNode.getNumber()) + " " + std::to_string(roughnessValueNode.getNumber());

			Image* image = findImageByName(mapKey);
			if (!image) {
				image = createImage(mapKey);
				image->width = 1;
				image->height = 1;
				image->format = ImageFormat::R8G8B8A8;
				image->colorSpace = ImageColorSpace::Linear;
				image->data = { roughnessValue, roughnessValue, roughnessValue, roughnessValue };
			}

			material.roughnessTexture.image = image;
		}
	}

	if (materialRoot.contains("occlusion")) {
		const JSON::Node& occlusionNode = materialRoot["occlusion"];

		if (occlusionNode.contains("texture")) {
			const JSON::Node& occlusionTextureNode = occlusionNode["texture"];

			if (occlusionTextureNode.contains("imagePath")) {
				material.occlusionTexture.image = loadImage(occlusionTextureNode["imagePath"].getString());
			}

			if (occlusionTextureNode.contains("imageSamplerPath")) {
				loadImageSamplerNtsp(occlusionTextureNode["imageSamplerPath"].getString(), material.occlusionTexture.imageSampler);
			}
		}
		else if (occlusionNode.contains("value")) {
			const JSON::Node& occlusionValueNode = occlusionNode["value"];
			uint8_t occlusionValue = static_cast<uint8_t>(round(255.0f * occlusionValueNode.getNumber()));

			std::string mapKey = "linear " + std::to_string(occlusionValueNode.getNumber()) + " " + std::to_string(occlusionValueNode.getNumber()) + " " + std::to_string(occlusionValueNode.getNumber()) + " " + std::to_string(occlusionValueNode.getNumber());

			Image* image = findImageByName(mapKey);
			if (!image) {
				image = createImage(mapKey);
				image->width = 1;
				image->height = 1;
				image->format = ImageFormat::R8G8B8A8;
				image->colorSpace = ImageColorSpace::Linear;
				image->data = { occlusionValue, occlusionValue, occlusionValue, occlusionValue };
			}

			material.occlusionTexture.image = image;
		}
	}

	if (materialRoot.contains("emissive")) {
		const JSON::Node& emissiveNode = materialRoot["emissive"];

		if (emissiveNode.contains("texture")) {
			const JSON::Node& emissiveTextureNode = emissiveNode["texture"];

			if (emissiveTextureNode.contains("imagePath")) {
				material.emissiveTexture.image = loadImage(emissiveTextureNode["imagePath"].getString());
			}

			if (emissiveTextureNode.contains("imageSamplerPath")) {
				loadImageSamplerNtsp(emissiveTextureNode["imageSamplerPath"].getString(), material.emissiveTexture.imageSampler);
			}
		}
		else if (emissiveNode.contains("color")) {
			const JSON::Node& emissiveColorNode = emissiveNode["color"];

			std::string mapKey = "srgb " + std::to_string(emissiveColorNode[0].getNumber()) + " " + std::to_string(emissiveColorNode[1].getNumber()) + " " + std::to_string(emissiveColorNode[2].getNumber()) + " " + std::to_string(1.0f);

			Image* image = findImageByName(mapKey);
			if (!image) {
				image = createImage(mapKey);
				image->width = 1;
				image->height = 1;
				image->format = ImageFormat::R8G8B8A8;
				image->colorSpace = ImageColorSpace::SRGB;
				image->data = { static_cast<uint8_t>(round(255.0f * emissiveColorNode[0].getNumber())), static_cast<uint8_t>(round(255.0f * emissiveColorNode[1].getNumber())), static_cast<uint8_t>(round(255.0f * emissiveColorNode[2].getNumber())), 255 };
			}

			material.emissiveTexture.image = image;
		}

		if (emissiveNode.contains("factor")) {
			material.emissiveFactor = emissiveNode["factor"].getNumber();
		}
	}

	if (materialRoot.contains("alphaCutoff")) {
		material.alphaCutoff = materialRoot["alphaCutoff"].getNumber();
	}

	if (materialRoot.contains("indexOfRefraction")) {
		material.indexOfRefraction = materialRoot["indexOfRefraction"].getNumber();
	}

	if (materialRoot.contains("useTriplanarMapping")) {
		material.useTriplanarMapping = materialRoot["useTriplanarMapping"].getBoolean();
	}

	if (materialRoot.contains("scaleUV")) {
		material.scaleUV.x = materialRoot["scaleUV"][0].getNumber();
		material.scaleUV.y = materialRoot["scaleUV"][1].getNumber();
	}

	if (materialRoot.contains("offsetUV")) {
		material.offsetUV.x = materialRoot["offsetUV"][0].getNumber();
		material.offsetUV.y = materialRoot["offsetUV"][1].getNumber();
	}
}

void NtshEngn::AssetManager::loadModelNtmd(const std::string& filePath, Model& model) {
	JSON json;
	const JSON::Node& modelRoot = json.read(filePath);

	if (modelRoot.contains("primitives")) {
		for (size_t i = 0; i < modelRoot["primitives"].size(); i++) {
			const JSON::Node& primitiveNode = modelRoot["primitives"][i];

			ModelPrimitive primitive;
			if (primitiveNode.contains("meshPath")) {
				loadMeshNtmh(primitiveNode["meshPath"].getString(), primitive.mesh);
			}
					
			if (primitiveNode.contains("materialPath")) {
				loadMaterialNtml(primitiveNode["materialPath"].getString(), primitive.material);
			}

			model.primitives.push_back(primitive);
		}
	}
}

void NtshEngn::AssetManager::loadImageNtim(const std::string& filePath, Image& image) {
	const std::unordered_map<std::string, ImageFormat> stringToImageFormat{
		{ "R8", ImageFormat::R8 },
		{ "R8G8", ImageFormat::R8G8 },
		{ "R8G8B8", ImageFormat::R8G8B8 },
		{ "R8G8B8A8", ImageFormat::R8G8B8A8 },
		{ "R16", ImageFormat::R16 },
		{ "R16G16", ImageFormat::R16G16 },
		{ "R16G16B16", ImageFormat::R16G16B16 },
		{ "R16G16B16A16", ImageFormat::R16G16B16A16 },
		{ "R32", ImageFormat::R32 },
		{ "R32G32", ImageFormat::R32G32 },
		{ "R32G32B32", ImageFormat::R32G32B32 },
		{ "R32G32B32A32", ImageFormat::R32G32B32A32 },
		{ "Unknown", ImageFormat::Unknown }
	};
	const std::unordered_map<std::string, ImageColorSpace> stringToImageColorSpace{
		{ "Linear", ImageColorSpace::Linear },
		{ "SRGB", ImageColorSpace::SRGB },
		{ "Unknown", ImageColorSpace::Unknown }
	};

	JSON json;
	const JSON::Node& imageRoot = json.read(filePath);

	if (imageRoot.contains("width")) {
		image.width = static_cast<uint32_t>(imageRoot["width"].getNumber());
	}
	if (imageRoot.contains("height")) {
		image.height = static_cast<uint32_t>(imageRoot["height"].getNumber());
	}
	if (imageRoot.contains("format")) {
		image.format = stringToImageFormat.at(imageRoot["format"].getString());
	}
	if (imageRoot.contains("colorSpace")) {
		image.colorSpace = stringToImageColorSpace.at(imageRoot["colorSpace"].getString());
	}
	if (imageRoot.contains("data")) {
		for (size_t i = 0; i < imageRoot["data"].size(); i++) {
			image.data.push_back(static_cast<uint8_t>(imageRoot["data"][i].getNumber()));
		}
	}
}

void NtshEngn::AssetManager::loadSoundNtsd(const std::string& filePath, Sound& sound) {
	JSON json;
	const JSON::Node& soundRoot = json.read(filePath);

	if (soundRoot.contains("channels")) {
		sound.channels = static_cast<uint8_t>(soundRoot["channels"].getNumber());
	}

	if (soundRoot.contains("sampleRate")) {
		sound.sampleRate = static_cast<int32_t>(soundRoot["sampleRate"].getNumber());
	}

	if (soundRoot.contains("bitsPerSample")) {
		sound.bitsPerSample = static_cast<uint8_t>(soundRoot["bitsPerSample"].getNumber());
	}

	if (soundRoot.contains("size")) {
		sound.size = static_cast<size_t>(soundRoot["size"].getNumber());
	}

	if (soundRoot.contains("data")) {
		for (size_t i = 0; i < soundRoot["data"].size(); i++) {
			sound.data.push_back(static_cast<uint8_t>(soundRoot["data"][i].getNumber()));
		}
	}
}

std::string NtshEngn::AssetManager::getNormalizedPath(const std::string& filePath) {
	if (filePath.empty()) {
		return "";
	}

	std::string normalizedPath = std::filesystem::canonical(filePath).string();
	std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
	std::string currentPath = std::filesystem::current_path().string();
	std::replace(currentPath.begin(), currentPath.end(), '\\', '/');
	if (normalizedPath.substr(0, currentPath.size()) == currentPath) {
		normalizedPath = normalizedPath.substr(currentPath.size() + 1);
	}

	return normalizedPath;
}

#pragma once
#include "../Common/asset_manager/ntshengn_asset_manager_interface.h"
#include "../Common/modules/ntshengn_asset_loader_module_interface.h"
#include "../resources/ntshengn_resources_audio.h"
#include "../resources/ntshengn_resources_graphics.h"
#include <filesystem>
#include <unordered_map>

namespace NtshEngn {

	class AssetManager : public AssetManagerInterface {
	public:
		Model* createModel(const std::string& modelName);
		Model* loadModel(const std::string& filePath);

		Material* createMaterial(const std::string& materialName);
		Material* loadMaterial(const std::string& filePath);

		Image* createImage(const std::string& imageName);
		Image* loadImage(const std::string& filePath);

		Font* createFont(const std::string& fontName);
		Font* loadFontBitmap(const std::string& filePath, float fontHeight);
		Font* loadFontSDF(const std::string& filePath);

		Sound* createSound(const std::string& soundName);
		Sound* loadSound(const std::string& filePath);

		void destroyModel(const std::string& modelName);
		void destroyMaterial(const std::string& materialName);
		void destroyImage(const std::string& imageName);
		void destroyFont(const std::string& fontName);
		void destroySound(const std::string& soundName);

		Model* findModelByName(const std::string& modelName);
		std::string getModelName(const Model* model);

		Material* findMaterialByName(const std::string& materialName);
		std::string getMaterialName(const Material* material);

		Image* findImageByName(const std::string& imageName);
		std::string getImageName(const Image* image);

		Font* findFontByName(const std::string& fontName);
		std::string getFontName(const Font* font);

		Sound* findSoundByName(const std::string& soundName);
		std::string getSoundName(const Sound* sound);

		void calculateTangents(Mesh& mesh);
		std::array<Math::vec3, 2> calculateAABB(const Mesh& mesh);

	public:
		void setAssetLoaderModule(AssetLoaderModuleInterface* assetLoaderModule);

	private:
		void loadMeshNtmh(const std::string& filePath, Mesh& mesh);

		void loadImageSamplerNtsp(const std::string& filePath, ImageSampler& imageSampler);

		void loadMaterialNtml(const std::string& filePath, Material& material);

		void loadModelNtmd(const std::string& filePath, Model& model);

		void loadImageNtim(const std::string& filePath, Image& image);

		void loadSoundNtsd(const std::string& filePath, Sound& sound);

		std::string getNormalizedPath(const std::string& filePath);

	private:
		AssetLoaderModuleInterface* m_assetLoaderModule;

		std::unordered_map<std::string, Model> m_models;
		std::unordered_map<std::string, Material> m_materials;
		std::unordered_map<std::string, Image> m_images;
		std::unordered_map<std::string, Font> m_fonts;
		std::unordered_map<std::string, Sound> m_sounds;

		std::unordered_map<Model*, std::string> m_modelNames;
		std::unordered_map<Material*, std::string> m_materialNames;
		std::unordered_map<Image*, std::string> m_imageNames;
		std::unordered_map<Font*, std::string> m_fontNames;
		std::unordered_map<Sound*, std::string> m_soundNames;

		std::unordered_map<std::string, std::filesystem::file_time_type> m_modelLastModified;
		std::unordered_map<std::string, std::filesystem::file_time_type> m_materialLastModified;
		std::unordered_map<std::string, std::filesystem::file_time_type> m_imageLastModified;
		std::unordered_map<std::string, std::filesystem::file_time_type> m_fontLastModified;
		std::unordered_map<std::string, std::filesystem::file_time_type> m_soundLastModified;
	};

}
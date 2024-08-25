#pragma once
#include "../../Common/asset_manager/ntshengn_asset_manager_interface.h"
#include "../../Common/modules/ntshengn_asset_loader_module_interface.h"
#include "../../Common/utils/ntshengn_utils_bimap.h"
#include "../resources/ntshengn_resources_audio.h"
#include "../resources/ntshengn_resources_graphics.h"
#include <forward_list>

namespace NtshEngn {

	class AssetManager : public AssetManagerInterface {
	public:
		Sound* createSound();
		Sound* loadSound(const std::string& filePath);

		Model* createModel();
		Model* loadModel(const std::string& filePath);

		Material* createMaterial();
		Material* loadMaterial(const std::string& filePath);

		Image* createImage();
		Image* loadImage(const std::string& filePath);

		Font* createFont();
		Font* loadFont(const std::string& filePath, float fontHeight);

		void destroySound(Sound* sound);
		void destroyModel(Model* model);
		void destroyMaterial(Material* material);
		void destroyImage(Image* image);
		void destroyFont(Font* font);

		void calculateTangents(Mesh& mesh);
		std::array<Math::vec3, 2> calculateAABB(const Mesh& mesh);

	public:
		void setAssetLoaderModule(AssetLoaderModuleInterface* assetLoaderModule);

	private:
		void loadSoundNtsd(const std::string& filePath, Sound& sound);

		void loadMeshNtmh(const std::string& filePath, Mesh& mesh);

		void loadImageSamplerNtsp(const std::string& filePath, ImageSampler& imageSampler);

		void loadMaterialNtml(const std::string& filePath, Material& material);

		void loadModelNtmd(const std::string& filePath, Model& model);

		void loadImageNtim(const std::string& filePath, Image& image);

	private:
		AssetLoaderModuleInterface* m_assetLoaderModule;

		std::forward_list<Sound> m_soundResources;
		std::forward_list<Model> m_modelResources;
		std::forward_list<Material> m_materialResources;
		std::forward_list<Image> m_imageResources;
		std::forward_list<Font> m_fontResources;

		Bimap<std::string, Sound*> m_soundPaths;
		Bimap<std::string, Model*> m_modelPaths;
		Bimap<std::string, Material*> m_materialPaths;
		Bimap<std::string, Image*> m_imagePaths;
		Bimap<std::string, Font*> m_fontPaths;
	};

}
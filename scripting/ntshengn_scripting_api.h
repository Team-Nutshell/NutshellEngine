#pragma once
#include "../Common/resources/ntshengn_resources_scripting.h"
#include "../Common/module_interfaces/ntshengn_graphics_module_interface.h"
#include "../Common/module_interfaces/ntshengn_physics_module_interface.h"
#include "../Common/module_interfaces/ntshengn_window_module_interface.h"
#include "../Common/module_interfaces/ntshengn_audio_module_interface.h"
#include "../Common/ecs/ntshengn_ecs.h"
#include "../Common/asset_manager/ntshengn_asset_manager.h"
#include "../Common/job_system/ntshengn_job_system.h"
#include "../frame_limiter/ntshengn_frame_limiter.h"
#include "../networking/ntshengn_networking.h"
#include "../scene_manager/ntshengn_scene_manager.h"

namespace NtshEngn {
	
	struct ScriptingAPI : public Script {
	protected:
		// Resources
		Model* loadModel(const std::string& filePath);
		MeshID getMeshID(const Mesh& mesh);
		Image* loadImage(const std::string& filePath);
		ImageID getImageID(const Image& image);
		Font* loadFont(const std::string& filePath, float fontHeight);
		FontID getFontID(const Font& font);
		Sound* loadSound(const std::string& filePath);
		SoundID getSoundID(const Sound& sound);

		// ECS
		Entity createEntity(const std::string& name = "");
		void destroyEntity(Entity entity);

		void destroyAllEntities();
		void destroyNonPersistentEntities();

		void setEntityName(Entity entity, const std::string& name);
		bool entityHasName(Entity entity);
		std::string getEntityName(Entity entity);
		Entity findEntityByName(const std::string& name);

		void setEntityPersistence(Entity entity, bool persistent);
		bool isEntityPersistent(Entity entity);

		template <typename T>
		void addEntityComponent(Entity entity, T component) {
			ecs->addComponent(entity, component);
		}
		template <typename T>
		void removeEntityComponent(Entity entity) {
			ecs->removeComponent<T>(entity);
		}
		template <typename T>
		bool hasEntityComponent(Entity entity) {
			return ecs->hasComponent<T>(entity);
		}
		template <typename T>
		T& getEntityComponent(Entity entity) {
			return ecs->getComponent<T>(entity);
		}

		// Input
		InputState getKeyState(InputKeyboardKey key, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		InputState getMouseButtonState(InputMouseButton mouseButton, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		void setCursorPosition(int x, int y, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		int getCursorPositionX(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		int getCursorPositionY(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		float getMouseScrollOffsetX(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		float getMouseScrollOffsetY(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		void setCursorVisibility(bool visible, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		bool isCursorVisible(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		std::vector<GamepadID> getConnectedGamepads();

		InputState getGamepadButtonState(GamepadID gamepadID, InputGamepadButton button);
		float getGamepadStickAxisX(GamepadID gamepadID, InputGamepadStick stick);
		float getGamepadStickAxisY(GamepadID gamepadID, InputGamepadStick stick);
		float getGamepadLeftTrigger(GamepadID gamepadID);
		float getGamepadRightTrigger(GamepadID gamepadID);

		std::string getGamepadName(GamepadID gamepadID);

		// Windows
		WindowID openWindow(int width, int height, const std::string& title);
		bool isWindowOpen(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		void closeWindow(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		WindowID getMainWindowID();
		size_t windowCount();

		void setWindowSize(int width, int height, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		int getWindowWidth(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		int getWindowHeight(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		
		void setWindowPosition(int x, int y, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		int getWindowPositionX(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		int getWindowPositionY(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		void setWindowFullscreen(bool fullscreen, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		bool isWindowFullscreen(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		void setWindowBorderless(bool borderless, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		bool isWindowBorderless(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		void setWindowResizable(bool resizable, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		bool isWindowResizable(WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		void setWindowTitle(const std::string& title, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);
		void setWindowIcon(const Image& image, WindowID windowID = NTSHENGN_WINDOW_UNKNOWN);

		int getMonitorWidth();
		int getMonitorHeight();
		int getMonitorRefreshRate();
		float getMonitorDisplayScaling();

		// Physics
		IntersectionInformation intersect(const ColliderShape* shape1, const ColliderShape* shape2);
		std::vector<RaycastInformation> raycast(const Math::vec3& rayOrigin, const Math::vec3& rayDirection, float tMin = 0.0001f, float tMax = 1000000.0f);

		// Audio
		void playSound(SoundID soundID);
		void pauseSound(SoundID soundID);
		void stopSound(SoundID soundID);

		bool isSoundPlaying(SoundID soundID);

		void setSoundGain(SoundID soundID, float newGain);
		float getSoundGain(SoundID soundID);

		void setSoundPitch(SoundID soundID, float newPitch);
		float getSoundPitch(SoundID soundID);

		// UI
		void drawUIText(FontID fontID, const std::string& text, const Math::vec2& position, const Math::vec4& color = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		void drawUILine(const Math::vec2& start, const Math::vec2& end, const Math::vec4& color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		void drawUIRectangle(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		void drawUIImage(ImageID imageID, ImageSamplerFilter imageSamplerFilter, const Math::vec2& position, float rotation = 0.0f, const Math::vec2& scale = Math::vec2(1.0f, 1.0f), const Math::vec4& color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	};

}
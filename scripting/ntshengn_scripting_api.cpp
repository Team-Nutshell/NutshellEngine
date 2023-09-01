#include "ntshengn_scripting_api.h"

NtshEngn::Model* NtshEngn::ScriptingAPI::loadModel(const std::string& filePath) {
	return assetManager->loadModel(filePath);
}

NtshEngn::MeshID NtshEngn::ScriptingAPI::getMeshID(const Mesh& mesh) {
	if (!graphicsModule) {
		return NTSHENGN_MESH_UNKNOWN;
	}

	return graphicsModule->load(mesh);
}

NtshEngn::Image* NtshEngn::ScriptingAPI::loadImage(const std::string& filePath) {
	return assetManager->loadImage(filePath);
}

NtshEngn::ImageID NtshEngn::ScriptingAPI::getImageID(const Image& image) {
	if (!graphicsModule) {
		return NTSHENGN_IMAGE_UNKNOWN;
	}

	return graphicsModule->load(image);
}

NtshEngn::Font* NtshEngn::ScriptingAPI::loadFont(const std::string& filePath, float fontHeight) {
	return assetManager->loadFont(filePath, fontHeight);
}

NtshEngn::FontID NtshEngn::ScriptingAPI::getFontID(const Font& font) {
	if (!graphicsModule) {
		return NTSHENGN_FONT_UNKNOWN;
	}

	return graphicsModule->load(font);
}

NtshEngn::Sound* NtshEngn::ScriptingAPI::loadSound(const std::string& filePath) {
	return assetManager->loadSound(filePath);
}

NtshEngn::SoundID NtshEngn::ScriptingAPI::getSoundID(const Sound& sound) {
	if (!audioModule) {
		return NTSHENGN_SOUND_UNKNOWN;
	}

	return audioModule->load(sound);
}

NtshEngn::Entity NtshEngn::ScriptingAPI::createEntity(const std::string& name) {
	if (name != "") {
		return ecs->createEntity(name);
	}

	return ecs->createEntity();
}

void NtshEngn::ScriptingAPI::destroyEntity(Entity entity) {
	ecs->destroyEntity(entity);
}

void NtshEngn::ScriptingAPI::destroyAllEntities() {
	ecs->destroyAllEntities();
}

void NtshEngn::ScriptingAPI::destroyNonPersistentEntities() {
	ecs->destroyNonPersistentEntities();
}

void NtshEngn::ScriptingAPI::setEntityName(Entity entity, const std::string& name) {
	ecs->setEntityName(entity, name);
}

bool NtshEngn::ScriptingAPI::entityHasName(Entity entity) {
	return ecs->entityHasName(entity);
}

std::string NtshEngn::ScriptingAPI::getEntityName(Entity entity) {
	return ecs->getEntityName(entity);
}

NtshEngn::Entity NtshEngn::ScriptingAPI::findEntityByName(const std::string& name) {
	return ecs->findEntityByName(name);
}

void NtshEngn::ScriptingAPI::setEntityPersistence(Entity entity, bool persistent) {
	return ecs->setEntityPersistence(entity, persistent);
}

bool NtshEngn::ScriptingAPI::isEntityPersistent(Entity entity) {
	return ecs->isEntityPersistent(entity);
}

NtshEngn::InputState NtshEngn::ScriptingAPI::getKeyState(InputKeyboardKey key, WindowID windowID) {
	if (!windowModule) {
		return NtshEngn::InputState::None;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getKeyState(windowID, key);
}

NtshEngn::InputState NtshEngn::ScriptingAPI::getMouseButtonState(InputMouseButton mouseButton, WindowID windowID) {
	if (!windowModule) {
		return NtshEngn::InputState::None;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getMouseButtonState(windowID, mouseButton);
}

void NtshEngn::ScriptingAPI::setCursorPosition(int x, int y, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setCursorPosition(windowID, x, y);
}

int NtshEngn::ScriptingAPI::getCursorPositionX(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getCursorPositionX(windowID);
}

int NtshEngn::ScriptingAPI::getCursorPositionY(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getCursorPositionY(windowID);
}

float NtshEngn::ScriptingAPI::getMouseScrollOffsetX(WindowID windowID) {
	if (!windowModule) {
		return 0.0f;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getMouseScrollOffsetX(windowID);
}

float NtshEngn::ScriptingAPI::getMouseScrollOffsetY(WindowID windowID) {
	if (!windowModule) {
		return 0.0f;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getMouseScrollOffsetY(windowID);
}

void NtshEngn::ScriptingAPI::setCursorVisibility(bool visible, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setCursorVisibility(windowID, visible);
}

bool NtshEngn::ScriptingAPI::isCursorVisible(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isCursorVisible(windowID);
}

std::vector<NtshEngn::GamepadID> NtshEngn::ScriptingAPI::getConnectedGamepads() {
	if (!windowModule) {
		return {};
	}

	return windowModule->getConnectedGamepads();
}

NtshEngn::InputState NtshEngn::ScriptingAPI::getGamepadButtonState(GamepadID gamepadID, InputGamepadButton button) {
	if (!windowModule) {
		return NtshEngn::InputState::None;
	}

	return windowModule->getGamepadButtonState(gamepadID, button);
}

float NtshEngn::ScriptingAPI::getGamepadStickAxisX(GamepadID gamepadID, InputGamepadStick stick) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadStickAxisX(gamepadID, stick);
}

float NtshEngn::ScriptingAPI::getGamepadStickAxisY(GamepadID gamepadID, InputGamepadStick stick) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadStickAxisY(gamepadID, stick);
}

float NtshEngn::ScriptingAPI::getGamepadLeftTrigger(GamepadID gamepadID) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadLeftTrigger(gamepadID);
}

float NtshEngn::ScriptingAPI::getGamepadRightTrigger(GamepadID gamepadID) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadRightTrigger(gamepadID);
}

std::string NtshEngn::ScriptingAPI::getGamepadName(GamepadID gamepadID) {
	if (!windowModule) {
		return "";
	}

	return windowModule->getGamepadName(gamepadID);
}

NtshEngn::WindowID NtshEngn::ScriptingAPI::openWindow(int width, int height, const std::string& title) {
	if (!windowModule) {
		return NTSHENGN_WINDOW_UNKNOWN;
	}

	return windowModule->openWindow(width, height, title);
}

bool NtshEngn::ScriptingAPI::isWindowOpen(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowOpen(windowID);
}

void NtshEngn::ScriptingAPI::closeWindow(WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->closeWindow(windowID);
}

NtshEngn::WindowID NtshEngn::ScriptingAPI::getMainWindowID() {
	if (!windowModule) {
		return NTSHENGN_WINDOW_UNKNOWN;
	}

	return windowModule->getMainWindowID();
}

size_t NtshEngn::ScriptingAPI::windowCount() {
	if (!windowModule) {
		return 0;
	}

	return windowModule->windowCount();
}

void NtshEngn::ScriptingAPI::setWindowSize(int width, int height, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowSize(windowID, width, height);
}

int NtshEngn::ScriptingAPI::getWindowWidth(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowWidth(windowID);
}

int NtshEngn::ScriptingAPI::getWindowHeight(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowHeight(windowID);
}

void NtshEngn::ScriptingAPI::setWindowPosition(int x, int y, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowPosition(windowID, x, y);
}

int NtshEngn::ScriptingAPI::getWindowPositionX(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowPositionX(windowID);
}

int NtshEngn::ScriptingAPI::getWindowPositionY(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowPositionY(windowID);
}

void NtshEngn::ScriptingAPI::setWindowFullscreen(bool fullscreen, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowFullscreen(windowID, fullscreen);
}

bool NtshEngn::ScriptingAPI::isWindowFullscreen(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowFullscreen(windowID);
}

void NtshEngn::ScriptingAPI::setWindowBorderless(bool borderless, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowBorderless(windowID, borderless);
}

bool NtshEngn::ScriptingAPI::isWindowBorderless(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowBorderless(windowID);
}

void NtshEngn::ScriptingAPI::setWindowResizable(bool resizable, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowResizable(windowID, resizable);
}

bool NtshEngn::ScriptingAPI::isWindowResizable(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowResizable(windowID);
}

void NtshEngn::ScriptingAPI::setWindowTitle(const std::string& title, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowTitle(windowID, title);
}

void NtshEngn::ScriptingAPI::setWindowIcon(const Image& image, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowIcon(windowID, image);
}

int NtshEngn::ScriptingAPI::getMonitorWidth() {
	if (!windowModule) {
		return 0 ;
	}

	return windowModule->getMonitorWidth();
}

int NtshEngn::ScriptingAPI::getMonitorHeight() {
	if (!windowModule) {
		return 0;
	}

	return windowModule->getMonitorHeight();
}

int NtshEngn::ScriptingAPI::getMonitorRefreshRate() {
	if (!windowModule) {
		return 0;
	}

	return windowModule->getMonitorRefreshRate();
}

float NtshEngn::ScriptingAPI::getMonitorDisplayScaling() {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getMonitorDisplayScaling();
}

NtshEngn::IntersectionInformation NtshEngn::ScriptingAPI::intersect(const ColliderShape* shape1, const ColliderShape* shape2) {
	if (!physicsModule) {
		return NtshEngn::IntersectionInformation();
	}

	return physicsModule->intersect(shape1, shape2);
}

std::vector<NtshEngn::RaycastInformation> NtshEngn::ScriptingAPI::raycast(const Math::vec3& rayOrigin, const Math::vec3& rayDirection, float tMin, float tMax) {
	if (!physicsModule) {
		return {};
	}

	return physicsModule->raycast(rayOrigin, rayDirection, tMin, tMax);
}

void NtshEngn::ScriptingAPI::playSound(SoundID soundID) {
	if (!audioModule) {
		return;
	}

	audioModule->play(soundID);
}

void NtshEngn::ScriptingAPI::pauseSound(SoundID soundID) {
	if (!audioModule) {
		return;
	}

	audioModule->pause(soundID);
}

void NtshEngn::ScriptingAPI::stopSound(SoundID soundID) {
	if (!audioModule) {
		return;
	}

	audioModule->stop(soundID);
}

bool NtshEngn::ScriptingAPI::isSoundPlaying(SoundID soundID) {
	if (!audioModule) {
		return false;
	}

	return audioModule->isPlaying(soundID);
}

void NtshEngn::ScriptingAPI::setSoundGain(SoundID soundID, float newGain) {
	if (!audioModule) {
		return;
	}

	audioModule->setGain(soundID, newGain);
}

float NtshEngn::ScriptingAPI::getSoundGain(SoundID soundID) {
	if (!audioModule) {
		return 0.0f;
	}

	return audioModule->getGain(soundID);
}

void NtshEngn::ScriptingAPI::setSoundPitch(SoundID soundID, float newPitch) {
	if (!audioModule) {
		return;
	}

	audioModule->setPitch(soundID, newPitch);
}

float NtshEngn::ScriptingAPI::getSoundPitch(SoundID soundID) {
	if (!audioModule) {
		return 0.0f;
	}

	return audioModule->getPitch(soundID);
}

void NtshEngn::ScriptingAPI::drawUIText(FontID fontID, const std::string& text, const Math::vec2& position, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUIText(fontID, text, position, color);
}

void NtshEngn::ScriptingAPI::drawUILine(const Math::vec2& start, const Math::vec2& end, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUILine(start, end, color);
}

void NtshEngn::ScriptingAPI::drawUIRectangle(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUIRectangle(position, size, color);
}

void NtshEngn::ScriptingAPI::drawUIImage(ImageID imageID, ImageSamplerFilter imageSamplerFilter, const Math::vec2& position, float rotation, const Math::vec2& scale, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUIImage(imageID, imageSamplerFilter, position, rotation, scale, color);
}

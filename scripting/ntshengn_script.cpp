#include "ntshengn_script.h"

NtshEngn::Model* NtshEngn::Script::loadModel(const std::string& filePath) {
	return assetManager->loadModel(filePath);
}

NtshEngn::MeshID NtshEngn::Script::getMeshID(const Mesh& mesh) {
	if (!graphicsModule) {
		return NTSHENGN_MESH_UNKNOWN;
	}

	return graphicsModule->load(mesh);
}

NtshEngn::Image* NtshEngn::Script::loadImage(const std::string& filePath) {
	return assetManager->loadImage(filePath);
}

NtshEngn::ImageID NtshEngn::Script::getImageID(const Image& image) {
	if (!graphicsModule) {
		return NTSHENGN_IMAGE_UNKNOWN;
	}

	return graphicsModule->load(image);
}

NtshEngn::Font* NtshEngn::Script::loadFont(const std::string& filePath, float fontHeight) {
	return assetManager->loadFont(filePath, fontHeight);
}

NtshEngn::FontID NtshEngn::Script::getFontID(const Font& font) {
	if (!graphicsModule) {
		return NTSHENGN_FONT_UNKNOWN;
	}

	return graphicsModule->load(font);
}

NtshEngn::Sound* NtshEngn::Script::loadSound(const std::string& filePath) {
	return assetManager->loadSound(filePath);
}

NtshEngn::SoundID NtshEngn::Script::getSoundID(const Sound& sound) {
	if (!audioModule) {
		return NTSHENGN_SOUND_UNKNOWN;
	}

	return audioModule->load(sound);
}

void NtshEngn::Script::goToScene(const std::string& filePath) {
	sceneManager->goToScene(filePath);
}

std::string NtshEngn::Script::getCurrentScenePath() {
	return sceneManager->getCurrentScenePath();
}

NtshEngn::Entity NtshEngn::Script::createEntity(const std::string& name) {
	if (name != "") {
		return ecs->createEntity(name);
	}

	return ecs->createEntity();
}

void NtshEngn::Script::destroyEntity(Entity entity) {
	ecs->destroyEntity(entity);
}

void NtshEngn::Script::destroyAllEntities() {
	ecs->destroyAllEntities();
}

void NtshEngn::Script::destroyNonPersistentEntities() {
	ecs->destroyNonPersistentEntities();
}

bool NtshEngn::Script::entityExists(Entity entity) {
	return ecs->entityExists(entity);
}

void NtshEngn::Script::setEntityName(Entity entity, const std::string& name) {
	ecs->setEntityName(entity, name);
}

bool NtshEngn::Script::entityHasName(Entity entity) {
	return ecs->entityHasName(entity);
}

std::string NtshEngn::Script::getEntityName(Entity entity) {
	return ecs->getEntityName(entity);
}

NtshEngn::Entity NtshEngn::Script::findEntityByName(const std::string& name) {
	return ecs->findEntityByName(name);
}

void NtshEngn::Script::setEntityPersistence(Entity entity, bool persistent) {
	return ecs->setEntityPersistence(entity, persistent);
}

bool NtshEngn::Script::isEntityPersistent(Entity entity) {
	return ecs->isEntityPersistent(entity);
}

NtshEngn::InputState NtshEngn::Script::getKeyState(InputKeyboardKey key, WindowID windowID) {
	if (!windowModule) {
		return NtshEngn::InputState::None;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getKeyState(windowID, key);
}

NtshEngn::InputState NtshEngn::Script::getMouseButtonState(InputMouseButton mouseButton, WindowID windowID) {
	if (!windowModule) {
		return NtshEngn::InputState::None;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getMouseButtonState(windowID, mouseButton);
}

void NtshEngn::Script::setCursorPosition(int x, int y, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setCursorPosition(windowID, x, y);
}

int NtshEngn::Script::getCursorPositionX(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getCursorPositionX(windowID);
}

int NtshEngn::Script::getCursorPositionY(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getCursorPositionY(windowID);
}

float NtshEngn::Script::getMouseScrollOffsetX(WindowID windowID) {
	if (!windowModule) {
		return 0.0f;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getMouseScrollOffsetX(windowID);
}

float NtshEngn::Script::getMouseScrollOffsetY(WindowID windowID) {
	if (!windowModule) {
		return 0.0f;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getMouseScrollOffsetY(windowID);
}

void NtshEngn::Script::setCursorVisibility(bool visible, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setCursorVisibility(windowID, visible);
}

bool NtshEngn::Script::isCursorVisible(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isCursorVisible(windowID);
}

std::vector<NtshEngn::GamepadID> NtshEngn::Script::getConnectedGamepads() {
	if (!windowModule) {
		return {};
	}

	return windowModule->getConnectedGamepads();
}

NtshEngn::InputState NtshEngn::Script::getGamepadButtonState(GamepadID gamepadID, InputGamepadButton button) {
	if (!windowModule) {
		return NtshEngn::InputState::None;
	}

	return windowModule->getGamepadButtonState(gamepadID, button);
}

float NtshEngn::Script::getGamepadStickAxisX(GamepadID gamepadID, InputGamepadStick stick) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadStickAxisX(gamepadID, stick);
}

float NtshEngn::Script::getGamepadStickAxisY(GamepadID gamepadID, InputGamepadStick stick) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadStickAxisY(gamepadID, stick);
}

float NtshEngn::Script::getGamepadLeftTrigger(GamepadID gamepadID) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadLeftTrigger(gamepadID);
}

float NtshEngn::Script::getGamepadRightTrigger(GamepadID gamepadID) {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getGamepadRightTrigger(gamepadID);
}

std::string NtshEngn::Script::getGamepadName(GamepadID gamepadID) {
	if (!windowModule) {
		return "";
	}

	return windowModule->getGamepadName(gamepadID);
}

NtshEngn::WindowID NtshEngn::Script::openWindow(int width, int height, const std::string& title) {
	if (!windowModule) {
		return NTSHENGN_WINDOW_UNKNOWN;
	}

	return windowModule->openWindow(width, height, title);
}

bool NtshEngn::Script::isWindowOpen(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowOpen(windowID);
}

void NtshEngn::Script::closeWindow(WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->closeWindow(windowID);
}

NtshEngn::WindowID NtshEngn::Script::getMainWindowID() {
	if (!windowModule) {
		return NTSHENGN_WINDOW_UNKNOWN;
	}

	return windowModule->getMainWindowID();
}

size_t NtshEngn::Script::windowCount() {
	if (!windowModule) {
		return 0;
	}

	return windowModule->windowCount();
}

void NtshEngn::Script::setWindowSize(int width, int height, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowSize(windowID, width, height);
}

int NtshEngn::Script::getWindowWidth(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowWidth(windowID);
}

int NtshEngn::Script::getWindowHeight(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowHeight(windowID);
}

void NtshEngn::Script::setWindowPosition(int x, int y, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowPosition(windowID, x, y);
}

int NtshEngn::Script::getWindowPositionX(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowPositionX(windowID);
}

int NtshEngn::Script::getWindowPositionY(WindowID windowID) {
	if (!windowModule) {
		return 0;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->getWindowPositionY(windowID);
}

void NtshEngn::Script::setWindowFullscreen(bool fullscreen, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowFullscreen(windowID, fullscreen);
}

bool NtshEngn::Script::isWindowFullscreen(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowFullscreen(windowID);
}

void NtshEngn::Script::setWindowBorderless(bool borderless, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowBorderless(windowID, borderless);
}

bool NtshEngn::Script::isWindowBorderless(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowBorderless(windowID);
}

void NtshEngn::Script::setWindowResizable(bool resizable, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowResizable(windowID, resizable);
}

bool NtshEngn::Script::isWindowResizable(WindowID windowID) {
	if (!windowModule) {
		return false;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	return windowModule->isWindowResizable(windowID);
}

void NtshEngn::Script::setWindowTitle(const std::string& title, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowTitle(windowID, title);
}

void NtshEngn::Script::setWindowIcon(const Image& image, WindowID windowID) {
	if (!windowModule) {
		return;
	}

	if (windowID == NTSHENGN_WINDOW_UNKNOWN) {
		windowID = windowModule->getMainWindowID();
	}

	windowModule->setWindowIcon(windowID, image);
}

int NtshEngn::Script::getMonitorWidth() {
	if (!windowModule) {
		return 0 ;
	}

	return windowModule->getMonitorWidth();
}

int NtshEngn::Script::getMonitorHeight() {
	if (!windowModule) {
		return 0;
	}

	return windowModule->getMonitorHeight();
}

int NtshEngn::Script::getMonitorRefreshRate() {
	if (!windowModule) {
		return 0;
	}

	return windowModule->getMonitorRefreshRate();
}

float NtshEngn::Script::getMonitorDisplayScaling() {
	if (!windowModule) {
		return 0.0f;
	}

	return windowModule->getMonitorDisplayScaling();
}

NtshEngn::IntersectionInformation NtshEngn::Script::intersect(const ColliderShape* shape1, const ColliderShape* shape2) {
	if (!physicsModule) {
		return NtshEngn::IntersectionInformation();
	}

	return physicsModule->intersect(shape1, shape2);
}

std::vector<NtshEngn::RaycastInformation> NtshEngn::Script::raycast(const Math::vec3& rayOrigin, const Math::vec3& rayDirection, float tMin, float tMax) {
	if (!physicsModule) {
		return {};
	}

	return physicsModule->raycast(rayOrigin, rayDirection, tMin, tMax);
}

void NtshEngn::Script::playSound(SoundID soundID) {
	if (!audioModule) {
		return;
	}

	audioModule->play(soundID);
}

void NtshEngn::Script::pauseSound(SoundID soundID) {
	if (!audioModule) {
		return;
	}

	audioModule->pause(soundID);
}

void NtshEngn::Script::stopSound(SoundID soundID) {
	if (!audioModule) {
		return;
	}

	audioModule->stop(soundID);
}

bool NtshEngn::Script::isSoundPlaying(SoundID soundID) {
	if (!audioModule) {
		return false;
	}

	return audioModule->isPlaying(soundID);
}

void NtshEngn::Script::setSoundGain(SoundID soundID, float newGain) {
	if (!audioModule) {
		return;
	}

	audioModule->setGain(soundID, newGain);
}

float NtshEngn::Script::getSoundGain(SoundID soundID) {
	if (!audioModule) {
		return 0.0f;
	}

	return audioModule->getGain(soundID);
}

void NtshEngn::Script::setSoundPitch(SoundID soundID, float newPitch) {
	if (!audioModule) {
		return;
	}

	audioModule->setPitch(soundID, newPitch);
}

float NtshEngn::Script::getSoundPitch(SoundID soundID) {
	if (!audioModule) {
		return 0.0f;
	}

	return audioModule->getPitch(soundID);
}

void NtshEngn::Script::drawUIText(FontID fontID, const std::string& text, const Math::vec2& position, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUIText(fontID, text, position, color);
}

void NtshEngn::Script::drawUILine(const Math::vec2& start, const Math::vec2& end, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUILine(start, end, color);
}

void NtshEngn::Script::drawUIRectangle(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUIRectangle(position, size, color);
}

void NtshEngn::Script::drawUIImage(ImageID imageID, ImageSamplerFilter imageSamplerFilter, const Math::vec2& position, float rotation, const Math::vec2& scale, const Math::vec4& color) {
	if (!graphicsModule) {
		return;
	}

	graphicsModule->drawUIImage(imageID, imageSamplerFilter, position, rotation, scale, color);
}

NtshEngn::Script::UIElementState NtshEngn::Script::drawUIButton(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color, InputMouseButton mouseButton) {
	if (!graphicsModule || !windowModule) {
		return UIElementState::None;
	}

	graphicsModule->drawUIRectangle(position, size, color);

	int cursorPositionX = windowModule->getCursorPositionX(windowModule->getMainWindowID());
	int cursorPositionY = windowModule->getCursorPositionY(windowModule->getMainWindowID());
	if ((cursorPositionX >= position.x) && (cursorPositionX <= (position.x + size.x)) &&
		(cursorPositionY >= position.y) && (cursorPositionY <= (position.y + size.y))) {
		if (getMouseButtonState(mouseButton) == InputState::Pressed) {
			return UIElementState::Pressed;
		}
		else if (getMouseButtonState(mouseButton) == InputState::Held) {
			return UIElementState::Held;
		}
		else if (getMouseButtonState(mouseButton) == InputState::Released) {
			return UIElementState::Released;
		}
		else {
			return UIElementState::Hovered;
		}
	}

	return UIElementState::None;
}

void NtshEngn::Script::setMaxFPS(uint32_t maxFPS) {
	frameLimiter->setMaxFPS(maxFPS);
}

uint32_t NtshEngn::Script::getMaxFPS() {
	return frameLimiter->getMaxFPS();
}

void NtshEngn::Script::executeJob(const std::function<void()>& job) {
	jobSystem->execute(job);
}

void NtshEngn::Script::dispatchJob(uint32_t jobCount, uint32_t jobsPerWorker, const std::function<void(JobDispatchArguments)>& job) {
	jobSystem->dispatch(jobCount, jobsPerWorker, job);
}

void NtshEngn::Script::waitAllThreads() {
	jobSystem->wait();
}

uint32_t NtshEngn::Script::getNumThreads() {
	return jobSystem->getNumThreads();
}

NtshEngn::ServerSocket* NtshEngn::Script::createServerSocket(uint16_t port, NetworkType networkType) {
	return networking->createServerSocket(port, networkType);
}

NtshEngn::ClientSocket* NtshEngn::Script::createClientSocket(NetworkType networkType) {
	return networking->createClientSocket(networkType);
}

void NtshEngn::Script::closeServerSocket(ServerSocket* serverSocket) {
	networking->closeServerSocket(serverSocket);
}

void NtshEngn::Script::closeClientSocket(ClientSocket* clientSocket) {
	networking->closeClientSocket(clientSocket);
}
#include "ntshengn_frame_limiter.h"
#include <thread>
#include <chrono>
#include <cstdint>

void NtshEngn::FrameLimiter::wait(double frameStart) {
	using namespace std::chrono_literals;

	if (m_maxFPS != 0) {
		const double currentTime = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
		const double maxFPSToMilliseconds = 1000.0 / static_cast<double>(m_maxFPS);
		const double deltaTime = currentTime - frameStart;
		if (deltaTime < maxFPSToMilliseconds) {
			if ((maxFPSToMilliseconds - deltaTime) > 2.0) {
				std::chrono::duration<double, std::milli> sleepTime = (maxFPSToMilliseconds - deltaTime - 3.0) * 1ms;
				std::this_thread::sleep_for(sleepTime);
			}
			while ((std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count() - frameStart) < maxFPSToMilliseconds);
		}
	}
}

void NtshEngn::FrameLimiter::setMaxFPS(uint32_t maxFPS) {
	m_maxFPS = maxFPS;
}

uint32_t NtshEngn::FrameLimiter::getMaxFPS() {
	return m_maxFPS;
}
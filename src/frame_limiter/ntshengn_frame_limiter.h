#pragma once
#include <cstdint>
#include <chrono>

namespace NtshEngn {

	class FrameLimiter {
	public:
		void wait(double frameStart) {
			if (m_maxFPS != 0) {
				double currentTime = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
				double maxFPSToMilliseconds = 1000.0 / static_cast<double>(m_maxFPS);
				if ((currentTime - frameStart) < maxFPSToMilliseconds) {
					double busyWaitingStart = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
					while ((std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count() - busyWaitingStart) < (maxFPSToMilliseconds - (currentTime - frameStart)));
				}
			}
		}

		void setMaxFPS(uint32_t maxFPS) {
			m_maxFPS = maxFPS;
		}

		uint32_t getMaxFPS() {
			return m_maxFPS;
		}

	private:
		uint32_t m_maxFPS = 0;
	};

}
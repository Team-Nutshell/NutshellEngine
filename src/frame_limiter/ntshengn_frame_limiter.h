#pragma once
#include <cstdint>
#include <chrono>

namespace NtshEngn {

	class FrameLimiter {
	public:
		void wait(double frameStart);

		void setMaxFPS(uint32_t maxFPS);
		uint32_t getMaxFPS();
		
	private:
		uint32_t m_maxFPS = 0;
	};

}
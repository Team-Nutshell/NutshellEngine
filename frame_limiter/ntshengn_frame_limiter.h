#pragma once
#include <cstdint>

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
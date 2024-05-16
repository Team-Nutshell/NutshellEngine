#pragma once
#include "../../Common/frame_limiter/ntshengn_frame_limiter_interface.h"

namespace NtshEngn {

	class FrameLimiter : public FrameLimiterInterface {
	public:
		void wait(double frameStart);

		void setMaxFPS(uint32_t maxFPS);
		uint32_t getMaxFPS();

	private:
		uint32_t m_maxFPS = 0;
	};

}
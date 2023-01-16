#pragma once
#include "../../external/Common/ecs/ntshengn_ecs.h"

namespace NtshEngn {

	class Scripting : public System {
	public:
		void init();
		void update(double dt);
		void destroy();

		void setECS(ECS* ecs);

	private:
		ECS* m_ecs;
	};

}
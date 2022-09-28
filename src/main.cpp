#include "utils/ntsh_core_defines.h"
#include "../external/Common/ntsh_engine_defines.h"
#include "../external/Common/ntsh_engine_enums.h"

int main()
{
	int a;
	NTSH_UNUSED(a);
	NTSH_ASSERT(false);
	NTSH_CORE_ERROR("NutshellEngine", NTSH_RESULT_UNKNOWN_ERROR);
}
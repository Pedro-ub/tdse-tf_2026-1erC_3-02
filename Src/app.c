#include "app.h"
#include "fsm.h"

void appUpdate(void)
{
	static bool initialized = false;

	if(!initialized){
		fsmInit();
		initialized = true;
	}

	fsmUpdate();
}

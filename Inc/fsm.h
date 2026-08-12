#ifndef FSM_H_
#define FSM_H_

#include <stdint.h>
#include <stdbool.h>
#include "queue.h"

typedef enum{
	FSM_STATE_INIT = 0,
	FSM_STATE_SETUP,
	FSM_STATE_NORMAL,
	FSM_STATE_FAULT,
	FSM_STATE_COUNT
} fsmState_t;

void       fsmInit(void);
void       fsmUpdate(void);
fsmState_t fsmGetState(void);

void       fsmForceFault(uint32_t errorCode);

#endif /* FSM_H_ */

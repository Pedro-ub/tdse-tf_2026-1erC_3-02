#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <stdint.h>


void schedulerUpdate(void);
void schedulerInit(void);
uint32_t schedulerGetWcetMicros(uint8_t taskIndex);

#endif /* TASK_SCHEDULER_H */

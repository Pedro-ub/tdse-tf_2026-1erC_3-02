#include "task_scheduler.h"
#include "tick.h"
#include "app.h"
#include "button.h"
#include "buzzer.h"
#include "dip_switch.h"
#include "game.h"
#include "menu.h"
#include "ldr.h"
#include "bluetooth.h"
#include "lcd.h"
#include <stdint.h>

typedef struct{
	void (*task)(void); //puntero a la funcion
	uint32_t period; //cada cuantos ms se ejecuta
	uint32_t lastTick; //ultima vez que se ejecuto
	uint32_t wcetCycles;
	uint32_t avgCycles;
} Task_t;

static Task_t tasks[] = {
		{appUpdate , 1, 0},
		{buttonUpdate , 1, 0},
		{buzzerUpdate , 1, 0},
		{dipSwitchUpdate,  1, 0},
		{gameUpdate, 1, 0},
		{menuUpdate, 1, 0},
		{ldrUpdate, 1, 0},
		{bluetoothUpdate,  1, 0},
		{lcdUpdate,        1, 0},
};

#define TASK_COUNT (sizeof(tasks) / sizeof(tasks[0]))


void schedulerInit(void)
{
	/* Habilita el contador de ciclos del DWT, una sola vez */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void schedulerUpdate(void)
{
	uint32_t tick_get = tickGet();
	for(uint8_t i = 0; i < TASK_COUNT; i++){
		if(tick_get - tasks[i].lastTick >= tasks[i].period){
			tasks[i].lastTick = tick_get;

			uint32_t start = DWT->CYCCNT;
			tasks[i].task();
			uint32_t elapsed = DWT->CYCCNT - start;

			if(elapsed > tasks[i].wcetCycles){
				tasks[i].wcetCycles = elapsed;
			}
			tasks[i].avgCycles = tasks[i].avgCycles - (tasks[i].avgCycles / 8u) + (elapsed / 8u);
		}
	}
}

uint32_t schedulerGetWcetMicros(uint8_t taskIndex)
{
	if(taskIndex >= TASK_COUNT) return 0;
	/* Convierte ciclos a microsegundos, según el reloj real (64MHz) */
	return tasks[taskIndex].wcetCycles / 64u;
}

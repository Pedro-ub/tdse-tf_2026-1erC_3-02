#include "tick.h"

volatile uint32_t tick_ms = 0;

void tick_callback(void)
{
	tick_ms++;
}

uint32_t tickGet(void)
{
	return tick_ms;
}

#include "low_power.h"
#include "main.h"

void lowPowerInit(void)
{

}

void lowPowerEnterIdle(void)
{
	/* Modo Sleep: apaga solo el nucleo del CPU, deja todos los
	 * perifericos activos (timers, UART, ADC, GPIO). Se despierta
	 * automaticamente ante CUALQUIER interrupcion, incluido el propio
	 * SysTick de 1ms -> no rompe el polling de botones/DIP switch,
	 * que sigue corriendo normalmente en cada tick. */
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

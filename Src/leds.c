#include "leds.h"
#include <stdbool.h>
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

typedef struct{
	TIM_HandleTypeDef *htim;
	uint32_t           channel;
	uint8_t            isComplementary; /* 1 si es un canal *N (TIM1_CHxN) */
} LedPwm_t;

/* Tabla de hardware: un LED por fila, con su timer/canal real.
 * TODO: si en el futuro se agrega un 5to LED de estado, sumarlo aca. */
static const LedPwm_t ledTable[LED_COUNT] = {
	{ &htim1, TIM_CHANNEL_2, 1 },  /* led_0 -> PB0,  TIM1_CH2N */
	{ &htim4, TIM_CHANNEL_3, 0 },  /* led_1 -> PB8,  TIM4_CH3 */
	{ &htim1, TIM_CHANNEL_1, 1 },  /* led_2 -> PA7,  TIM1_CH1N */
	{ &htim2, TIM_CHANNEL_3, 0 },  /* led_3 -> PB10, TIM2_CH3 */
};

static bool    ledState[LED_COUNT];
static uint8_t brightnessPercent = 100;

static void applyDuty(uint8_t id, uint8_t percent)
{
	uint32_t arr = ledTable[id].htim->Init.Period; /* 65535 en los 3 timers */
	uint32_t ccr = ((uint32_t)percent * arr) / 100u;
	__HAL_TIM_SET_COMPARE(ledTable[id].htim, ledTable[id].channel, ccr);
}

void ledInit(void)
{
	for(uint8_t i = 0; i < LED_COUNT; i++){
		if(ledTable[i].isComplementary){
			HAL_TIMEx_PWMN_Start(ledTable[i].htim, ledTable[i].channel);
		} else {
			HAL_TIM_PWM_Start(ledTable[i].htim, ledTable[i].channel);
		}
	}
	ledAllOff();
}

void ledOn(uint8_t id)
{
	if(id >= LED_COUNT) return;
	ledState[id] = true;
	applyDuty(id, brightnessPercent);
}

void ledOff(uint8_t id)
{
	if(id >= LED_COUNT) return;
	ledState[id] = false;
	applyDuty(id, 0);
}

void ledToggle(uint8_t id)
{
	if(id >= LED_COUNT) return;
	if(ledState[id]){
		ledOff(id);
	} else {
		ledOn(id);
	}
}

void ledAllOff(void)
{
	for(uint8_t i = 0; i < LED_COUNT; i++){
		ledOff(i);
	}
}

void ledSetBrightness(uint8_t percent)
{
	if(percent > 100) percent = 100;
	brightnessPercent = percent;

	/* Reaplica el nuevo brillo solo a los LEDs que ya estaban encendidos */
	for(uint8_t i = 0; i < LED_COUNT; i++){
		if(ledState[i]){
			applyDuty(i, brightnessPercent);
		}
	}
}

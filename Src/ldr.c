#include "ldr.h"
#include "main.h"
#include "leds.h"

extern ADC_HandleTypeDef hadc1;

#define ADC_MAX_VALUE        4095u
#define ADC_DEADBAND_PERCENT 2u

static volatile uint16_t adcRawValue = 0;
static uint8_t  brightnessPercent = 100;

static uint16_t mapAdcToPercent(uint16_t raw)
{
	uint32_t percent = ((uint32_t)raw * 100u) / ADC_MAX_VALUE;
	return (uint16_t)percent;
}

void ldrInit(void)
{
	adcRawValue = 0;
	brightnessPercent = 100;
	HAL_ADC_Start_IT(&hadc1);   /* arranca la primera conversion por interrupcion */
}

void ldrUpdate(void)
{
	uint16_t newPercent = mapAdcToPercent(adcRawValue);

	uint16_t diff = (newPercent > brightnessPercent)
	                ? (newPercent - brightnessPercent)
	                : (brightnessPercent - newPercent);

	if(diff >= ADC_DEADBAND_PERCENT){
		brightnessPercent = (uint8_t)newPercent;
		ledSetBrightness(brightnessPercent);
	}
}

uint8_t ldrGetBrightnessPercent(void)
{
	return brightnessPercent;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1){
		adcRawValue = HAL_ADC_GetValue(hadc);
		HAL_ADC_Start_IT(&hadc1);   /* re-arma la siguiente conversion */
	}
}

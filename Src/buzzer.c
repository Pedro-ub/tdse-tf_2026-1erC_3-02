#include "buzzer.h"
#include "main.h"
#include "tick.h"


extern TIM_HandleTypeDef htim3;
#define BUZZER_TIM        (&htim3)
#define BUZZER_CHANNEL    TIM_CHANNEL_1




typedef struct{
	uint32_t freq_hz;
	uint32_t duration_ms;
} BuzzerSoundDef_t;

/* Tabla de sonidos: frecuencia y duracion de cada evento */
static const BuzzerSoundDef_t soundTable[] = {
	[BUZZER_SOUND_NONE]  = {0,    0},
	[BUZZER_SOUND_HIT]   = {1500, 80},
	[BUZZER_SOUND_MISS]  = {300,  200},
	[BUZZER_SOUND_MENU]  = {800,  40},
	[BUZZER_SOUND_FAULT] = {200,  500},
};

static bool     playing = false;
static uint32_t stopTick = 0;


#define TIMER_CLOCK_HZ_BASE  64000000UL
#define BUZZER_PRESCALER      63u
#define TIMER_CLOCK_HZ        (TIMER_CLOCK_HZ_BASE / (BUZZER_PRESCALER + 1))

static void buzzerSetFrequency(uint32_t freq_hz)
{
	if(freq_hz == 0) return;
	uint32_t arr = (TIMER_CLOCK_HZ / freq_hz) - 1;
	__HAL_TIM_SET_PRESCALER(BUZZER_TIM, BUZZER_PRESCALER);
	__HAL_TIM_SET_AUTORELOAD(BUZZER_TIM, arr);
	__HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CHANNEL, arr / 2);
}

void buzzerInit(void)
{
	playing = false;
	stopTick = 0;
	HAL_TIM_PWM_Stop(BUZZER_TIM, BUZZER_CHANNEL);
}

void buzzerPlay(buzzer_sound_t sound)
{
	if(sound == BUZZER_SOUND_NONE) return;

	const BuzzerSoundDef_t *def = &soundTable[sound];
	if(def->freq_hz == 0 || def->duration_ms == 0) return;

	buzzerSetFrequency(def->freq_hz);
	HAL_TIM_PWM_Start(BUZZER_TIM, BUZZER_CHANNEL);

	playing = true;
	stopTick = tickGet() + def->duration_ms;
}

void buzzerStop(void)
{
	HAL_TIM_PWM_Stop(BUZZER_TIM, BUZZER_CHANNEL);
	playing = false;
}

void buzzerUpdate(void)
{
	if(playing && (tickGet() >= stopTick)){
		buzzerStop();
	}
}

bool buzzerIsPlaying(void)
{
	return playing;
}

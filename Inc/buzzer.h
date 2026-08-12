#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stdbool.h>

/* Sonidos predefinidos del sistema. Cada uno tiene su propia
 * frecuencia y duracion, definidas en buzzer.c */
typedef enum{
	BUZZER_SOUND_NONE = 0,
	BUZZER_SOUND_HIT,      /* acierto en el juego */
	BUZZER_SOUND_MISS,     /* error / timeout */
	BUZZER_SOUND_MENU,     /* navegacion de menu */
	BUZZER_SOUND_FAULT     /* modo FALLA */
} buzzer_sound_t;

void buzzerInit(void);
void buzzerUpdate(void);
void buzzerPlay(buzzer_sound_t sound);
void buzzerStop(void);
bool buzzerIsPlaying(void);

#endif

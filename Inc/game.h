#ifndef GAME_H_
#define GAME_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum{
	GAME_DIFFICULTY_EASY = 0,
	GAME_DIFFICULTY_NORMAL,
	GAME_DIFFICULTY_HARD,
	GAME_DIFFICULTY_COUNT
} gameDifficulty_t;

void gameSetDifficulty(uint8_t level);

void gameInit(void);


void gameStart(void);


void gameUpdate(void);


void gameStop(void);

uint16_t gameGetScore(void);
uint8_t  gameGetLives(void);
const char* gameGetDifficultyName(void);

#endif /* GAME_H_ */

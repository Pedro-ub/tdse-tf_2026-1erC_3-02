#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>
#include <stdbool.h>

#define STORAGE_RANKING_SIZE 3u

void storageInit(void);

uint16_t storageGetDifficulty(void);
void storageSetDifficulty(uint16_t level);


const uint16_t* storageGetRanking(uint8_t *count);


bool storageSubmitScore(uint16_t score);

void storageResetRanking(void);

#endif /* STORAGE_H_ */

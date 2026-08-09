#include "storage.h"
#include "main.h"
#include <string.h>

/* Ultima pagina de 1KB de la flash interna (128KB totales en el F103RB).
 */
#define STORAGE_FLASH_ADDR   0x0801FC00u
#define STORAGE_MAGIC        0xBEEFu
#define STORAGE_VERSION      1u


#define STORAGE_WORD_COUNT (3u + STORAGE_RANKING_SIZE)

static uint16_t cache[STORAGE_WORD_COUNT];

static void storageFlush(void)
{
	FLASH_EraseInitTypeDef eraseInit;
	uint32_t pageError = 0;

	HAL_FLASH_Unlock();

	eraseInit.TypeErase   = FLASH_TYPEERASE_PAGES;
	eraseInit.PageAddress = STORAGE_FLASH_ADDR;
	eraseInit.NbPages     = 1;
	HAL_FLASHEx_Erase(&eraseInit, &pageError);

	for(uint32_t i = 0; i < STORAGE_WORD_COUNT; i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,
		                   STORAGE_FLASH_ADDR + (i * 2u),
		                   cache[i]);
	}

	HAL_FLASH_Lock();
}

static void storageLoadDefaults(void)
{
	cache[0] = STORAGE_MAGIC;
	cache[1] = STORAGE_VERSION;
	cache[2] = 1u; /* GAME_DIFFICULTY_NORMAL */
	for(uint8_t i = 0; i < STORAGE_RANKING_SIZE; i++){
		cache[3 + i] = 0u;
	}
}

void storageInit(void)
{
	const uint16_t *flashPtr = (const uint16_t *)STORAGE_FLASH_ADDR;

	if(flashPtr[0] != STORAGE_MAGIC || flashPtr[1] != STORAGE_VERSION){
		/* primera vez que arranca, o version vieja incompatible */
		storageLoadDefaults();
		storageFlush();
	} else {
		memcpy(cache, flashPtr, STORAGE_WORD_COUNT * sizeof(uint16_t));
	}
}

uint16_t storageGetDifficulty(void)
{
	return cache[2];
}

void storageSetDifficulty(uint16_t level)
{
	cache[2] = level;
	storageFlush();
}

const uint16_t* storageGetRanking(uint8_t *count)
{
	if(count != NULL){
		*count = STORAGE_RANKING_SIZE;
	}
	return &cache[3];
}

bool storageSubmitScore(uint16_t score)
{
	uint16_t *ranking = &cache[3];
	uint8_t insertPos = STORAGE_RANKING_SIZE; /* STORAGE_RANKING_SIZE = "no entro" */

	for(uint8_t i = 0; i < STORAGE_RANKING_SIZE; i++){
		if(score > ranking[i]){
			insertPos = i;
			break;
		}
	}

	if(insertPos >= STORAGE_RANKING_SIZE){
		return false;
	}

	for(uint8_t i = STORAGE_RANKING_SIZE - 1; i > insertPos; i--){
		ranking[i] = ranking[i - 1];
	}
	ranking[insertPos] = score;

	storageFlush();
	return true;
}

void storageResetRanking(void)
{
	for(uint8_t i = 0; i < STORAGE_RANKING_SIZE; i++){
		cache[3 + i] = 0u;
	}
	storageFlush();
}

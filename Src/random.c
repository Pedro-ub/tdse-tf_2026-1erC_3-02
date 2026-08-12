#include "random.h"

static uint32_t state = 1; /* nunca puede ser 0, xorshift se rompe con seed=0 */

void randomInit(uint32_t seed)
{
	state = (seed != 0) ? seed : 0xA53C9F17u; /* fallback si llega semilla 0 */
}

void randomReseed(uint32_t entropy)
{
	/* Mezclamos la entropía nueva con el estado actual en vez de
	 * pisarlo, así no perdemos aleatoriedad ya acumulada. */
	state ^= entropy;
	if(state == 0){
		state = 0xA53C9F17u;
	}
}

uint32_t randomGet(void)
{
	state ^= (state << 13);
	state ^= (state >> 17);
	state ^= (state << 5);
	return state;
}

uint8_t randomRange(uint8_t min, uint8_t max)
{
	if(min >= max) return min;
	uint32_t span = (uint32_t)(max - min) + 1u;
	return (uint8_t)(min + (randomGet() % span));
}

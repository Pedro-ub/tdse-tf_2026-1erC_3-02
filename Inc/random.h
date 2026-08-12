#ifndef RANDOM_H_
#define RANDOM_H_

#include <stdint.h>


void randomInit(uint32_t seed);



void randomReseed(uint32_t entropy);

uint32_t randomGet(void);


uint8_t randomRange(uint8_t min, uint8_t max);

#endif /* RANDOM_H_ */

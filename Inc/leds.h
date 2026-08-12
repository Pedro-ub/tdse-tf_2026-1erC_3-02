#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>
#include "main.h"

#define LED_COUNT 4u

void ledInit(void);
void ledOn(uint8_t id);
void ledOff(uint8_t id);
void ledToggle(uint8_t id);
void ledAllOff(void);


void ledSetBrightness(uint8_t percent);

#endif /* LEDS_H_ */

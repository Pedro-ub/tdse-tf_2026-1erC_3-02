#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"


typedef enum task_sensor_ev {EV_BTN_UP,
							 EV_BTN_DOWN} button_ev;


typedef enum task_sensor_st {ST_BTN_UP,
							 ST_BTN_DOWN,
							 ST_BTN_RISING,
							 ST_BTN_FALLING} button_st;


typedef enum task_sensor_id {ID_BTN_A,
							ID_BTN_B,
							ID_BTN_C,
							ID_BTN_D} button_id;

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
	button_id id;
} Button_C;

typedef struct
{
	uint32_t			tick;
	button_st       	state;
	button_ev       	event;
} Button_V;

void buttonInit(void);
void buttonUpdate(void);
bool buttonGetEvent(uint8_t id);




#endif

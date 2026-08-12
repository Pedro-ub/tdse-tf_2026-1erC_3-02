#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdbool.h>


typedef enum{
	    EVENT_NONE = 0,

		/* Botones */
		EVENT_BTN_A_PRESSED,
		EVENT_BTN_B_PRESSED,
		EVENT_BTN_C_PRESSED,
		EVENT_BTN_D_PRESSED,

		/* DIP switches */
		EVENT_DIP_CHANGED,

		/* Juego */
		EVENT_GAME_TIMEOUT,
		EVENT_GAME_OVER,


		EVENT_LDR_SAMPLE_READY,

		/* Bluetooth */
		EVENT_BT_DATA_RX,
		EVENT_BT_CONFIG_RECEIVED,

		/* Fallos genericos de hardware (cualquier modulo puede reportarlo) */
		EVENT_HW_FAULT,

		EVENT_MENU_START_GAME,

} EventType_t;


typedef struct{
	EventType_t type;
	uint32_t    data;
} Event_t;

void queueInit(void);
bool queuePush(Event_t event);
bool queuePop(Event_t *event);
bool queueIsEmpty(void);
bool queueIsFull(void);

#endif

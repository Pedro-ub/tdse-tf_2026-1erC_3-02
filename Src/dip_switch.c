#include "dip_switch.h"
#include "main.h"
#include "tick.h"
#include "config.h"
#include "queue.h"

typedef struct{
	GPIO_TypeDef *port;
	uint16_t      pin;
} DipHW_t;

/* Tabla de hardware, análoga a leds.c / button.c.
  */
static const DipHW_t dipTable[DIP_SWITCH_QTY] = {
	{ DIP_0_GPIO_Port, DIP_0_Pin },  /* DIP0 */
	{ DIP_1_GPIO_Port, DIP_1_Pin },  /* DIP1 */
	{ DIP_2_GPIO_Port, DIP_2_Pin },  /* DIP2 */
	{ DIP_3_GPIO_Port, DIP_3_Pin  },  /* DIP3 */
};

typedef enum{
	ST_DIP_STABLE,
	ST_DIP_DEBOUNCING
} dipState_t;

typedef struct{
	dipState_t state;
	bool       lastRead;   /* última lectura cruda del pin */
	bool       stableVal;  /* valor confirmado tras antirrebote */
	uint32_t   tick;
} DipV_t;

static DipV_t dipList[DIP_SWITCH_QTY];
static volatile bool changedFlag = false;

/* Activo-alto o activo-bajo depende del cableado; acá asumo activo-alto
 * (switch ON = 3.3V).  */
static bool dipReadRaw(uint8_t i)
{
	return (HAL_GPIO_ReadPin(dipTable[i].port, dipTable[i].pin) == GPIO_PIN_RESET);
}

void dipSwitchInit(void)
{
	for(uint8_t i = 0; i < DIP_SWITCH_QTY; i++){
		bool raw = dipReadRaw(i);
		dipList[i].state     = ST_DIP_STABLE;
		dipList[i].lastRead  = raw;
		dipList[i].stableVal = raw;
		dipList[i].tick      = 0;
	}
	changedFlag = false;
}

void dipSwitchUpdate(void)
{
	for(uint8_t i = 0; i < DIP_SWITCH_QTY; i++){

		bool raw = dipReadRaw(i);

		switch(dipList[i].state){

		case ST_DIP_STABLE:
			if(raw != dipList[i].stableVal){
				dipList[i].state    = ST_DIP_DEBOUNCING;
				dipList[i].lastRead = raw;
				dipList[i].tick     = tickGet();
			}
			break;

		case ST_DIP_DEBOUNCING:
			if(raw != dipList[i].lastRead){
				dipList[i].lastRead = raw;
				dipList[i].tick     = tickGet();
			} else if((tickGet() - dipList[i].tick) >= DIP_DEBOUNCE_MS){
				dipList[i].stableVal = raw;
				dipList[i].state     = ST_DIP_STABLE;
				changedFlag = true;

				Event_t ev = { .type = EVENT_DIP_CHANGED, .data = i };
				queuePush(ev);
			}
			break;

		default:
			dipList[i].state = ST_DIP_STABLE;
			break;
		}
	}
}

bool dipSwitchRead(uint8_t id)
{
	if(id >= DIP_SWITCH_QTY) return false;
	return dipList[id].stableVal;
}

uint8_t dipSwitchReadAll(void)
{
	uint8_t mask = 0;
	for(uint8_t i = 0; i < DIP_SWITCH_QTY; i++){
		if(dipList[i].stableVal){
			mask |= (1u << i);
		}
	}
	return mask;
}

bool dipSwitchChanged(void)
{
	return changedFlag;
}

void dipSwitchClearChanged(void)
{
	changedFlag = false;
}

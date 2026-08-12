#ifndef DIP_SWITCH_H_
#define DIP_SWITCH_H_

#include <stdint.h>
#include <stdbool.h>

#define DIP_SWITCH_QTY   4u
typedef enum{
	ID_DIP_0 = 0,
	ID_DIP_1,
	ID_DIP_2,
	ID_DIP_3
} dipId_t;

void    dipSwitchInit(void);
void    dipSwitchUpdate(void);
bool    dipSwitchRead(uint8_t id);
uint8_t dipSwitchReadAll(void);
bool    dipSwitchChanged(void);
void    dipSwitchClearChanged(void);

#endif /* DIP_SWITCH_H_ */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include <stdbool.h>

void bluetoothInit(void);
void bluetoothUpdate(void);

/* Envia el score actual a la app, formato de texto simple */
void bluetoothSendScore(uint16_t score);

#endif /* BLUETOOTH_H_ */

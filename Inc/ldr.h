#ifndef LDR_H_
#define LDR_H_

#include <stdint.h>

void ldrInit(void);
void ldrUpdate(void);


uint8_t ldrGetBrightnessPercent(void);

#endif /* LDR_H_ */

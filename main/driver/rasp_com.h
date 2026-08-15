#ifndef _RASP_COM_H_
#define _RASP_COM_H_

#include "stdint.h"

void rasp_com_init(void);

int rasp_2_stm32(uint8_t *ret);
int stm32_2_rasp(uint8_t *buff);


#endif
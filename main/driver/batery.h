#ifndef _BATERY_H_
#define _BATERY_H_

#include "adc.h"


void batery_init(void);
void batery_read(uint32_t *ret);

#endif
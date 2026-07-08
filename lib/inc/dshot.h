#ifndef _DSHOT_H_
#define _DSHOT_H_

#include <stdint.h>
#include "gpio.h"


#define LENGTH_DSHOT_DATA                   52


typedef enum {
    DShot150 = 0,
    DShot300,
    DShot600,
    DShot1200,
} dshot_type_t;


typedef struct {
    dshot_type_t dshot_type;
    gpio_port gpio_port;
    uint8_t pin_start;
} dshot_handle_t;

/* tim 1 only */
int dshot_init(dshot_handle_t cfg);
void dshot_stop(void);
void dshot_start(void);
void dshot_write(uint16_t dc1, uint16_t dc2, uint16_t dc3, uint16_t dc4);

#endif
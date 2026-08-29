#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdint.h>


typedef enum {
    TIMER2 = 0,
    TIMER3,
    TIMER4,
    TIMER5,
    TIMER6,
    TIMER7,
    TIMER12,
    TIMER13,
    TIMER14,
} timer_general_type_t;


void timer_init();


void delay_us(volatile uint32_t time);
void delay_ms(volatile uint32_t time);


uint32_t GET_CURRENT_US();

void START_SYSTEM_TIMER();
void STOP_SYSTEM_TIMER();
void RESET_SYSTEM_TIMER();




#endif
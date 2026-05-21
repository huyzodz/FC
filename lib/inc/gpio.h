#ifndef _GPIO_H_
#define _GPIO_H_


#include <stdio.h>
#include <stdint.h>


typedef enum {
    GPIO_INPUT = 0,
    GPIO_INPUT_PULLUP ,
    GPIO_INPUT_PULLDOWN,
    GPIO_OUTPUT_PULL_UP,
    GPIO_OUTPUT_OPEN_DRAIN,
    GPIO_ALTERNATE_INPUT,
    GPIO_ALTERNATE_INPUT_PULL_UP,
    GPIO_ALTERNATE_INPUT_PULL_DOWN,
    GPIO_ALTERNATE_OUTPUT_PUSH_PULL,
    GPIO_ALTERNATE_OUTPUT_OPEN_DRAIN
} gpio_mode;


/* read page 77 in datasheet */
typedef enum {
    AF0 = 0,
    AF1,
    AF2,
    AF3,
    AF4,
    AF5,
    AF6,
    AF7,
    AF8,
    AF9,
    AF10,
    AF11,
    AF12,
    AF13,
    AF14,
    AF15
} alternate_mode;

typedef enum {
    GPIOA = 0,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI,
    GPIOJ,
    GPIOK
} gpio_port;

typedef enum {
    low_speed = 0,
    medium_speed,
    high_speed,
    very_high_speed
} gpio_output_speed;

typedef struct {
    gpio_port gpio;
    uint8_t pinNum;
    gpio_mode mode;
    alternate_mode alternate; // write AF0 if not use
    gpio_output_speed OutSpeed;
} gpio_handle_t;


void gpio_init(gpio_handle_t *handle);
void gpio_write(gpio_port gpio, uint8_t pin, uint8_t level);
uint16_t gpio_read(gpio_port gpio, uint8_t pin);


#endif

#include "main.h"


void test(void)
{
    /* test */
    gpio_handle_t test = {
        .mode = GPIO_OUTPUT_PULL_UP,
        .gpio = GPIOA,
        .pinNum = 4,
        .OutSpeed = medium_speed
    };
    gpio_init(&test);
    gpio_write(GPIOA, 4, 1);
}

void setup(void)
{
    uint32_t clk, res;
    /* clock init must be call at first */
    clock_init(CLOCK_400_MHZ);
    /* check clock */
    SystemCoreClockUpdate();
    clk = SystemCoreClock;
}

int main(void)

{
	
    /* dump for loop to keep stm busy a while for erase if needed when code reg dump */
    for (uint64_t i = 0;i < 10000000;i++);

    
    setup();
    /* function for testing */
	test();
	while (1);

    return 0;
	
}

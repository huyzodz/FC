#include "main.h"
#include <string.h>



void test(void)
{
    /* test */
    /*
    gpio_config_t test = {
        .mode = GPIO_OUTPUT_PULL_UP,
        .gpio = GPIO_PORT_A,
        .pinNum = 5,
        .OutSpeed = medium_speed
    };
    gpio_init(&test);
    gpio_write(GPIO_PORT_A, 5, 1);

    usart_config_t usart_cfg = {
        .baudrate = USART_BAUDRATE_115200,
        .intr_en = USART_FALSE,
        .msb = USART_FALSE,
        .parity = USART_NONE_PARITY,
        .stop_bit = USART_1_STOP_BITS,
        .usart_num = USART_2,
        .word_length = USART_8_BITS,
        .gpio_port = GPIO_PORT_D,
        .pin_rx = 6,
        .pin_tx = 5,
        .dma_channel_rx = DMA_MUX_CHANNEL_1,
        .dma_channel_tx = DMA_MUX_CHANNEL_2,
        .size = 15,
        .data = usart_data
    };
    usart_init(usart_cfg);


    uint8_t data_tx [] = "hi_im_huy";
    usart_write("\r\n", strlen("\r\n"), USART_2);

    for (uint64_t i = 0;i < 10000000;i++);

    usart_write(data_tx, strlen(data_tx), USART_2);
    while (1)
    {
        usart_read(usart_data_read, 5, USART_2);
    }
    */
    

    //test_i2c();
    //test_bmi270();
    test_task_imu();
}

int main(void)
{
	
    /* dump for loop to keep stm busy a while for erase if needed when code reg dump */
    for (volatile uint32_t i = 0;i < 10000000;i++);

    // init here
    system_init();
	
	
	// test
	test();

    return 0;
	
}

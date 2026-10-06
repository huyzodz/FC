#include "batery.h"
#include "gpio.h"

#define ADC_DMA_READ                        DMA_MUX_CHANNEL_9


#define CUR_CHANNEL_BATERY                  ADC_CHANNEL_3
#define CUR_SAMPLE_TIM_BATERY               ADC_810_5_CLK
#define CUR_GPIO_PIN                        6
#define CUR_GPIO_PORT                       GPIO_PORT_A

#define VBAT_CHANNEL_BATERY                 ADC_CHANNEL_7
#define VBAT_SAMPLE_TIM_BATERY              ADC_810_5_CLK
#define VBAT_GPIO_PIN                       7
#define VBAT_GPIO_PORT                      GPIO_PORT_A

void batery_init(void)
{
	
	// init gpio
    gpio_config_t gpio_cfg = {
        .alternate = AF0,
        .gpio = ADC_GPIO_PORT,
        .mode = GPIO_ANALOG,
        .pinNum = ADC_GPIO_PIN
    };
    gpio_init(&gpio_cfg);
	
    adc_cfg_t cfg = {
        .dma_channel = ADC_DMA_READ,
        .hardware_calib = ADC_TRUE,
        .watchdog_en = ADC_FALSE,
        .num = ADC_NUM1,
        .oversampling = ADC_OVS_64,
        .res = ADC_14_BIT
    };

    adc_init(cfg);


    adc_channel_cfg_t bat = {
        .channel = ADC_CHANNEL_BATERY,
        .differential_mode_en = ADC_FALSE,
        .num = ADC_NUM1,
        .sample = ADC_SAMPLE_TIM_BATERY
    };

    adc_add_channel(bat);

    
    
}

void batery_read(uint32_t *ret)
{
    adc_read(ADC_CHANNEL_BATERY, ret);
}
#include "adc.h"
#include "timer.h"
#include <stdlib.h>


#define ADC_STOP(ptr)                       ((ptr)->CR |= (1 << 4))
#define ADC_START(ptr)                      ((ptr)->CR |= (1 << 2))


volatile ADC_TypeDef *arr_glb_adc [2] = {ADC1, ADC2};

volatile uint8_t list_channel_pos_read [20];
volatile uint32_t *adc_data_store = NULL;
volatile uint8_t number_channel_present = 0;
volatile dma_mux1_channel_t glb_channel;



void adc_dma_init(adc_cfg_t adc_cfg)
{
    dma_config_t cfg = {
        .dma_channel = adc_cfg.dma_channel,
        .dma_cir_mode = DMA_TRUE,
        .dma_interupt_enable = DMA_FALSE,
        .dma_mem_increse = DMA_TRUE,
        .dma_mem_size = WORD_32_BIT,
        .dma_per_increse = DMA_FALSE,
        .dma_per_size = WORD_32_BIT,
        .dma_priority = VERY_HIGH,
        .dma_tranfer_direction = PER_TO_MEM,
    };

    if (adc_cfg.num == ADC_NUM1)
        cfg.dma_request = ADC1_DMA;
    else 
        cfg.dma_request = ADC2_DMA;

    glb_channel = cfg.dma_channel;
    dma_init(cfg);
}

void adc_init(adc_cfg_t cfg)
{
    adc_num_t num = cfg.num;
    adc_bool_t watchdog = cfg.watchdog_en;
    adc_resolution_t resolution = cfg.res;
    adc_ovs_t oversampling = cfg.oversampling;
    uint16_t temp_ovs = 1;
    adc_bool_t calib = cfg.hardware_calib;
    ADC_TypeDef *adc = arr_glb_adc[num];
    
    
    // enable clock
    // this clock en for both adc1 and adc2
    RCC->AHB1ENR |= (1 << 5);

    // common cfg for adc
    // enable VREFEN (vref in)
    ADC12_COMMMON->CCR |= (1 << 22);

    // config for adc
    if (watchdog == ADC_TRUE)
    {
        // enable watch dog threshold and use for all channel
        adc->CFGR |= (1 << 23);
        adc->CFGR &= ~(1 << 22);

        // write num to low high thres
        adc->LTR1 = cfg.low_thres;
        adc->HTR1 = cfg.high_thres;
    }
    else
        adc->CFGR &= ~(1 << 23);

    // continue mode
    adc->CFGR |= (1 << 13);
    adc->CFGR &= ~(1 << 16);


    // auto delay for protect data to safe to read
    adc->CFGR |= (1 << 14);

    // enable overun mode 
    adc->CFGR |= (1 << 12);

    // data resolution
    // this use for all channel
    // reset
    adc->CFGR &= ~(0x07 << 2);
    adc->CFGR |= (resolution << 2);

    // enable dma for read
    adc->CFGR |= (0x03);

    // oversampling
    // reset
    adc->CFGR2 &= ~(0x3FF << 16);
    // calculate ovs
    for (int i = 0;i < oversampling;i++)
        temp_ovs *= 4;
    adc->CFGR2 |= ((temp_ovs - 1) << 16);

    // this config will have no pesudo bit
    // so it will right shift exatly with bit ovs
    // reset
    adc->CFGR2 &= ~(0x0F << 5);
    adc->CFGR2 |= ((oversampling*2) << 5);

    // enable oversampling
    adc->CFGR2 |= 0x01;

    // turn of deep power down
    adc->CR &= ~(1 << 29);

    // enable adc voltage
    adc->CR |= (1 << 28);
    // base on datasheet page 157 need max 10us to wait
    // but here delay for 50us
    delay_us(50);

    // calib here
    if (calib == ADC_TRUE)
    {
        // use for single-end mode
        adc->CR &= ~(1 << 30);
        // start calib
        adc->CR |= (1 << 31);
        // wait for calib
        while (((adc->CR >> 31) & 0x01) == 0x01);
    }

    // set 0 to all REG except ADVREGEN
    adc->CR &= (1 << 28);

    // clear flag ready if have
    adc->ISR |= (0x01); // write 1 to clear
    // wait clear
    while ((adc->ISR & 0x01) == 0x01);

    // en adc
    adc->CR |= 0x01;

    // wait start
    while ((adc->ISR & 0x01) != 0x01);

    // enable boost
    // beacuse adc clock source is per ck = 64Mhz
    adc->CR |= (1 << 8);    

    adc_dma_init(cfg);


    // not start here
}

void adc_add_channel(adc_channel_cfg_t cfg)
{
    ADC_TypeDef *adc = arr_glb_adc[cfg.num];
    adc_channel_t channel = cfg.channel;
    uint32_t *ptr_temp;
    uint8_t temp;

    // only 16 conversion sp
    if (number_channel_present >= 16)
        return;

    // force to stop adc to start add channel
    ADC_STOP(adc);
    // wait to stop
    while (((adc->CR >> 4) & 0x01) == 0x01);

    // add to preselect to read adc of that channel
    adc->PCSEL |= (1 << channel);

    // set number conversion
    adc->SQR1 &= ~(0x0F); // reset
    adc->SQR1 |= number_channel_present;

    // set location to read
    // this ponter will hold reg which location for next conversion
    if (number_channel_present < 4)\
    {
        temp = 0;
        ptr_temp = &adc->SQR1;
    }
    else if (number_channel_present < 9)
    {
        temp = 5;
        ptr_temp = &adc->SQR2;
    }
    else if (number_channel_present < 14)
    {
        temp = 10;
        ptr_temp = &adc->SQR3;
    }
    else
    {
        temp = 15;
        ptr_temp = &adc->SQR4;
    }

    // increse num chn
    number_channel_present++;
    temp = number_channel_present - temp;
    // reset for sure
    // multi 6 because 6 bit for 1 SQ
    *ptr_temp &= ~(0x1F << (6*temp));
    *ptr_temp |= (channel << (6*temp));


    // update dma
    dma_stop(glb_channel);
    // erase and create data
    if (adc_data_store != NULL)
    {
        free(adc_data_store);
        adc_data_store = NULL;
    }
    adc_data_store = (uint32_t*)malloc(sizeof(uint32_t) * number_channel_present);

    // set dma
    dma_SetAddr(&adc->DR, (uint32_t*)adc_data_store, number_channel_present, glb_channel);
    dma_start(glb_channel);

    // update pos read
    // set this to easily read data
    list_channel_pos_read[channel] = (number_channel_present - 1);


    // start adc
    adc->CR |= (1 << 2);
}

int adc_read(adc_channel_t channel, uint32_t *ret)
{
    *ret = adc_data_store[list_channel_pos_read[channel]];
    return 0;
}
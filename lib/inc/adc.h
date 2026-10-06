#ifndef _ADC_H_
#define _ADC_H_


#include "stm32h750vbt6.h"
#include "dma.h"

typedef enum {
    ADC_FALSE = 0,
    ADC_TRUE
} adc_bool_t;

typedef enum {
    ADC_NUM1 = 0,
    ADC_NUM2
} adc_num_t;

typedef enum {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15,
    ADC_CHANNEL_16,
    ADC_CHANNEL_17,
    ADC_CHANNEL_18,
    ADC_CHANNEL_19
} adc_channel_t;
/*
typedef enum {
    ADC_SQ1 = 6,
    ADC_SQ2 = 12,
    ADC_SQ3 = 18,
    ADC_SQ4 = 24,
    ADC_SQ5 = 0,
    ADC_SQ6 = 6,
    ADC_SQ7 = 12,
    ADC_SQ8 = 18,
    ADC_SQ9 = 24,
    ADC_SQ10 = 0,
    ADC_SQ11 = 6,
    ADC_SQ12 = 12,
    ADC_SQ13 = 18,
    ADC_SQ14 = 24,
    ADC_SQ15 = 0,
    ADC_SQ16 = 6,
} adc_position_read;
*/


typedef enum {
    ADC_16_BIT = 0,
    ADC_14_BIT,
    ADC_12_BIT,
    ADC_10_BIT,
    ADC_18_BIT,
} adc_resolution_t;

typedef enum {
    ADC_OVS_1 = 0,
    ADC_OVS_4,
    ADC_OVS_16,
    ADC_OVS_64,
    ADC_OVS_256,
    ADC_OVS_1024,
} adc_ovs_t;

typedef enum {
    ADC_1_5_CLK = 0,
    ADC_2_5_CLK,
    ADC_8_5_CLK,
    ADC_16_5_CLK,
    ADC_32_5_CLK,
    ADC_64_5_CLK,
    ADC_387_5_CLK,
    ADC_810_5_CLK,
} adc_channel_sampleTime_t;

typedef struct {
    adc_num_t num;
    adc_resolution_t res;
    adc_bool_t watchdog_en;
    adc_bool_t hardware_calib;
    

    // auto en continue mode
    dma_mux1_channel_t dma_channel;

    // from 0 -> 1023
    adc_ovs_t oversampling;

    // use to set range for watchdog
    // this base on resolution value
    uint32_t low_thres;
    uint32_t high_thres;


} adc_cfg_t;


typedef struct {
    adc_num_t num;
    adc_channel_t channel;
    adc_channel_sampleTime_t sample;

    // this mode = false that mean in and GND
    // if on, this mode will compare between in+ and in-
    // right now still not sp this mode
    adc_bool_t differential_mode_en;
} adc_channel_cfg_t;

// need to call first before add channel
// still not sp adc2
void adc_init(adc_cfg_t cfg);

// only support 16 channel
void adc_add_channel(adc_channel_cfg_t cfg);


int adc_read(adc_channel_t channel, uint32_t *ret);



#endif
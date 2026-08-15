#ifndef _DMA_H_
#define _DMA_H_

#include <stdint.h>


/*
    channel 0 dshot

*/

/* use to connect event to trig dma tranfer */
typedef enum {
    DMAMUX1_REQ_GEN0 = 1,
    DMAMUX1_REQ_GEN1,
    DMAMUX1_REQ_GEN2,
    DMAMUX1_REQ_GEN3,
    DMAMUX1_REQ_GEN4,
    DMAMUX1_REQ_GEN5,
    DMAMUX1_REQ_GEN6,
    DMAMUX1_REQ_GEN7,

    ADC1_DMA,
    ADC2_DMA,

    TIM1_CH1,
    TIM1_CH2,
    TIM1_CH3,
    TIM1_CH4,
    TIM1_UP,
    TIM1_TRIG,
    TIM1_COM,

    TIM2_CH1,
    TIM2_CH2,
    TIM2_CH3,
    TIM2_CH4,
    TIM2_UP,

    TIM3_CH1,
    TIM3_CH2,
    TIM3_CH3,
    TIM3_CH4,
    TIM3_UP,
    TIM3_TRIG,

    TIM4_CH1,
    TIM4_CH2,
    TIM4_CH3,
    TIM4_UP,

    I2C1_RX_DMA,
    I2C1_TX_DMA,
    I2C2_RX_DMA,
    I2C2_TX_DMA,

    SPI1_RX_DMA,
    SPI1_TX_DMA,
    SPI2_RX_DMA,
    SPI2_TX_DMA,

    USART1_RX_DMA,
    USART1_TX_DMA,
    USART2_RX_DMA,
    USART2_TX_DMA,
    USART3_RX_DMA,
    USART3_TX_DMA,

    TIM8_CH1,
    TIM8_CH2,
    TIM8_CH3,
    TIM8_CH4,
    TIM8_UP,
    TIM8_TRIG,
    TIM8_COM,

    RESERVED54,

    TIM5_CH1,
    TIM5_CH2,
    TIM5_CH3,
    TIM5_CH4,
    TIM5_UP,
    TIM5_TRIG,

    SPI3_RX_DMA,
    SPI3_TX_DMA,

    UART4_RX_DMA,
    UART4_TX_DMA,
    UART5_RX_DMA,
    UART5_TX_DMA,

    DAC_CH1_DMA,
    DAC_CH2_DMA,

    TIM6_UP,
    TIM7_UP,

    USART6_RX_DMA,
    USART6_TX_DMA,

    I2C3_RX_DMA,
    I2C3_TX_DMA,

    DCMI_DMA,
    CRYP_IN_DMA,
    CRYP_OUT_DMA,
    HASH_IN_DMA,

    UART7_RX_DMA,
    UART7_TX_DMA,
    UART8_RX_DMA,
    UART8_TX_DMA,

    SPI4_RX_DMA,
    SPI4_TX_DMA,
    SPI5_RX_DMA,
    SPI5_TX_DMA,

    SAI1A_DMA,
    SAI1B_DMA,
    SAI2A_DMA,
    SAI2B_DMA,

    SWPMI_RX_DMA,
    SWPMI_TX_DMA,

    SPDIFRX_DAT_DMA,
    SPDIFRX_CTRL_DMA,

    HR_REQ1,
    HR_REQ2,
    HR_REQ3,
    HR_REQ4,
    HR_REQ5,
    HR_REQ6,

    DFSDM1_DMA0,
    DFSDM1_DMA1,
    DFSDM1_DMA2,
    DFSDM1_DMA3,

    TIM15_CH1,
    TIM15_UP,
    TIM15_TRIG,
    TIM15_COM,

    TIM16_CH1,
    TIM16_UP,

    TIM17_CH1,
    TIM17_UP,

    SAI3_A_DMA,
    SAI3_B_DMA,

    ADC3_DMA

} dma_mux_request_t;


typedef enum {
    DMA_MUX_CHANNEL_0 = 0,
    DMA_MUX_CHANNEL_1,
    DMA_MUX_CHANNEL_2,
    DMA_MUX_CHANNEL_3,
    DMA_MUX_CHANNEL_4,
    DMA_MUX_CHANNEL_5,
    DMA_MUX_CHANNEL_6,
    DMA_MUX_CHANNEL_7,

    DMA_MUX_CHANNEL_8,
    DMA_MUX_CHANNEL_9,
    DMA_MUX_CHANNEL_10,
    DMA_MUX_CHANNEL_11,
    DMA_MUX_CHANNEL_12,
    DMA_MUX_CHANNEL_13,
    DMA_MUX_CHANNEL_14,
    DMA_MUX_CHANNEL_15
} dma_mux1_channel_t;

typedef enum {
    LOW = 0,
    MEDIUM,
    HIGH,
    VERY_HIGH
} dma_priority_t;

typedef enum {
    BYTE_8_BIT = 0,
    HALF_WORD_16_BIT,
    WORD_32_BIT
} dma_target_size_t;

typedef enum {
    DMA_FALSE = 0,
    DMA_TRUE
} dma_bool_t;

typedef enum {
    PER_TO_MEM = 0,
    MEM_TO_PER,
    MEM_TO_MEM
} dma_dir_t;

/* typedef for call back */
typedef void (*dma_callback_t)(void *arg);


typedef struct {
    dma_target_size_t dma_per_size;     /* size of peripheral */
    dma_target_size_t dma_mem_size;     /* size of mem */

    dma_bool_t dma_mem_increse;         /* mem increse or not*/
    dma_bool_t dma_per_increse;         /* peripheral increse or not */
    dma_bool_t dma_cir_mode;            /* circular mode is tranfer done and return to begin continue tranfer */

    dma_bool_t dma_interupt_enable;     /* enable intr or not this must be true if want to intr */
    dma_bool_t complete_tranfer_intr;   /* intr for complete tranfer */
    dma_bool_t half_tranfer_intr;       /* intr for half tranfer */
    dma_bool_t error_tranfer_intr;      /* intr when error tranfer */

    dma_priority_t dma_priority;        /* set priority */
    dma_mux_request_t dma_request;      /* select source to catch event trig dma */
    dma_mux1_channel_t dma_channel;     /* channel select for dma */
    dma_dir_t dma_tranfer_direction;    /* select direction when tranfer */
} dma_config_t;

typedef struct {
    dma_mux1_channel_t channel;
    dma_callback_t cb;

    void *data;
}dma_callback_handle_t;


typedef struct {
    dma_bool_t complete_flag;
    dma_bool_t half_flag;
    dma_bool_t error_flag;

    void *data;
} dma_flag_callback_t;



void dma_init(dma_config_t cfg);
void dma_add_callback(dma_callback_handle_t cfg);
void dma_start(dma_mux1_channel_t channel);
void dma_stop(dma_mux1_channel_t channel);
void dma_SetAddr(void *src, void *des, uint16_t length, dma_mux1_channel_t channel);
uint8_t dma_busy_check(dma_mux1_channel_t channel);
uint32_t dma_get_ndtr(dma_mux1_channel_t channel);

#endif

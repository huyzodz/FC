#include "i2c_master.h"
#include "clock.h"
#include "stm32h750vbt6.h"
#include "dma.h"
#include "timer.h"


/* (ns) if change this value, goto stm32CUBE to calculate again timing, it has tool to do this*/
#define MODE_100KHZ_RASI_TIME           200
#define MODE_100KHZ_FALL_TIME           100
#define TIMING_100KHZ                   0x80101212UL

#define MODE_400KHZ_RASI_TIME           100
#define MODE_400KHZ_FALL_TIME           70
#define TIMING_400KHZ                   0x00701139UL

#define MODE_1000KHZ_RASI_TIME          70
#define MODE_1000KHZ_FALL_TIME          70
#define TIMING_1000KHZ                  0x00400510UL


#define I2C_TIMEOUT                     100000

volatile i2c_bool_t FLAG_BURST_READ_DONE = I2C_FALSE;


#define I2C_SET_WRITE_MODE(i2c)         ((i2c)->CR2 &= ~(0x01 << 10))
#define I2C_SET_READ_MODE(i2c)          ((i2c)->CR2 |= (0x01 << 10))
#define I2C_SET_POINTER(i2c)            (i2c = (I2C_TypeDef*)(I2C1_BASE + (i2c_num*0x400)))

#define I2C_CLEAR_ALL_FLAG(i2c)         ((i2c)->ICR |= 0xF38)



static I2C_TypeDef* const i2c_table [] = {I2C1, I2C2, I2C3};





void i2c_clear_flag_stop(I2C_TypeDef *i2c)
{
    uint32_t i = 0;
    /* if out timer out then return -1*/
    /* wait until flag stop appear */
    while (!((i2c->ISR >> 5) & 0x01))
    {
        i++;
        if (i > I2C_TIMEOUT)          
            break;
    }
    // add latency
    delay_us(5);
    /* clear flag*/
    i2c->ICR |= (1 << 5);
}


int i2c_set_address(uint8_t addr, i2c_num_t i2c_num)
{
    I2C_TypeDef *i2c;
    I2C_SET_POINTER(i2c);

    /* return if in middle tranmission */
    /* bit 15 is bit busy */
    if ((i2c->ISR >> 15) & 0x01)
        return -1;
    

    /* reset address */
    i2c->CR2 &= ~(0x3FF);
    i2c->CR2 |= ((uint32_t)addr << 1);



    return 0;
}



void i2c_callback_handle(void *arg)
{
    dma_flag_callback_t *data = (dma_flag_callback_t*)arg;
    /* clear flag */
    if (data->complete_flag)
    {
        I2C_TypeDef *i2c = (I2C_TypeDef*)data->data;
        FLAG_BURST_READ_DONE = I2C_TRUE;
        i2c_clear_flag_stop(i2c);
    }
}


/* should pull up resistor 2->5kom*/
void i2c_gpio_init(gpio_port port, uint8_t gpio_scl, uint8_t gpio_sda)
{
    gpio_config_t scl = {
        .mode = GPIO_ALTERNATE_OUTPUT_OPEN_DRAIN,
        .pinNum = gpio_scl,
        .gpio = port,
        .OutSpeed = very_high_speed,
        .alternate = AF4
    };

    gpio_config_t sda = {
        .mode = GPIO_ALTERNATE_OUTPUT_OPEN_DRAIN,
        .pinNum = gpio_sda,
        .gpio = port,
        .OutSpeed = very_high_speed,
        .alternate = AF4
    };

    gpio_init(&scl);
    gpio_init(&sda);
}


void i2c_dma_init(dma_mux1_channel_t i2c_dma_rx, i2c_num_t i2c_num)
{
    dma_config_t dma_rx = {
        .dma_channel = i2c_dma_rx,
        .dma_cir_mode = DMA_FALSE,
        .dma_interupt_enable = DMA_TRUE,
        .complete_tranfer_intr = DMA_TRUE,
        .dma_mem_increse = DMA_TRUE,
        .dma_mem_size = BYTE_8_BIT,
        .dma_per_increse = DMA_FALSE,
        .dma_per_size = BYTE_8_BIT,
        .dma_priority = VERY_HIGH,
        .dma_tranfer_direction = PER_TO_MEM
    };


    /* check i2c that match dma request */
    if (i2c_num == I2C_NUM_1)
        dma_rx.dma_request = I2C1_RX_DMA;
    else if (i2c_num == I2C_NUM_2)
        dma_rx.dma_request = I2C2_RX_DMA;
    else if (i2c_num == I2C_NUM_3)
        dma_rx.dma_request = I2C3_RX_DMA;


    // init dma
    dma_init(dma_rx);

    dma_callback_handle_t cb = {
        .cb = &i2c_callback_handle,
        .channel = i2c_dma_rx,
        .data = i2c_table[i2c_num]
    };

    dma_add_callback(cb);
}

void i2c_init(i2c_config_t cfg)
{
    i2c_num_t num = cfg.i2c_num;
    I2C_TypeDef *i2c;
    i2c_mode_t mode = cfg.i2c_mode;
    i2c_bool_t analog_filter = cfg.analog_filter;

    i2c_bool_t irq_en = cfg.irq_en;
    dma_mux1_channel_t i2c_dma_rx = cfg.dma_read;


    /*
    if (num == I2C_NUM_4)
        i2c = I2C4;
    else
        i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));
    */
    i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));

    gpio_port port = cfg.port;
    uint8_t gpio_scl = cfg.gpio_scl, gpio_sda = cfg.gpio_sda;
    /* init gpio for i2c */
    i2c_gpio_init(port, gpio_scl, gpio_sda);


    /* enable clk */
    /*
    if (num < I2C_NUM_4)
        RCC->APB1LENR |= (1 << (num + 21));       
    lse
        RCC->APB4ENR |= (1 << 7); // i2c4
    */
    RCC->APB1LENR |= (1 << (num + 21));

    /* dis analog filter*/
    if (!analog_filter)
        i2c->CR1 |= (1 << 12);
    
    /* enable dma read*/
    i2c->CR1 |= (1 << 15);


    /* enable irq */
    if (irq_en)
    {

    }

    /* timing */
    if (mode == I2C_MODE_100KHZ)
        i2c->TIMINGR = TIMING_100KHZ;
    else if (mode == I2C_MODE_400KHZ)
        i2c->TIMINGR = TIMING_400KHZ;
    else 
        i2c->TIMINGR = TIMING_1000KHZ;

    // disable PE
    i2c->CR1 &= ~(0x01 << 23);


    /* init dma for read */
    i2c_dma_init(i2c_dma_rx, num);

    
    /* enable i2c */ 
    i2c->CR1 |= 0x01; 
}





/* only use if not set autoend */
void i2c_stop_tranfer(i2c_num_t i2c_num)
{
    I2C_TypeDef *i2c;
    i2c = (I2C_TypeDef*)(I2C1_BASE + (i2c_num*0x400));

    /* force stop */
    i2c->CR2 |= (0x01 << 14);
    i2c_clear_flag_stop(i2c);
}

int i2c_start(i2c_num_t i2c_num, uint16_t num_byte_tranfer, i2c_mode_tranfer_t mode, uint8_t addr_dev, i2c_bool_t auto_end)
{
    I2C_TypeDef *i2c;
    i2c = (I2C_TypeDef*)(I2C1_BASE + (i2c_num*0x400));

    /* return -1 if fail in set address or not stop */
    if (i2c_set_address(addr_dev, i2c_num) == -1 || (i2c->ISR >> 5) & 0x01)
        return -1;

    /* set mode tranfer */
    if (mode == MODE_WRITE)
        I2C_SET_WRITE_MODE(i2c);
    else
        I2C_SET_READ_MODE(i2c);

    /* set byte tranfer */
    i2c->CR2 &= ~(0xFF << 16); // reset bit
    if (num_byte_tranfer <= 255)
        i2c->CR2 |= (num_byte_tranfer << 16); // set byte tranfer
    else
    {
        // handle when need tranfer > 255 bytes
        i2c->CR2 |= (0xFF << 16); // set byte tranfer
        i2c->CR2 |= (0x01 << 24); // enable reload bit
    }

    if (auto_end == I2C_TRUE)
        /* set auto end */
        i2c->CR2 |= (0x01 << 25);
    else
        /* disable auto end */
        i2c->CR2 &= ~(0x01 << 25);

    /* start tranfer */
    i2c->CR2 |= (0x01 << 13);

    // return 0 if not err
    return 0;
}


int i2c_restart(i2c_num_t i2c_num, uint8_t num_byte_tranfer, i2c_mode_tranfer_t mode, i2c_bool_t auto_end)
{
    I2C_TypeDef *i2c;
    i2c = (I2C_TypeDef*)(I2C1_BASE + (i2c_num*0x400));


    /* wait tranfer complete */
    int i = 0;
    while (!((i2c->ISR>>6) & 0x01))
    {
        if (i++ > I2C_TIMEOUT)
            return -1;
    }

    /* set mode tranfer */
    if (mode == MODE_WRITE)
        I2C_SET_WRITE_MODE(i2c);
    else
        I2C_SET_READ_MODE(i2c);

    /* set byte tranfer */
    i2c->CR2 &= ~(0xFF << 16); // reset bit
    i2c->CR2 |= (num_byte_tranfer << 16); // 2 byte tranfer, 1 for address reg, 1 for val

    // handle auto set stop condition
    if (auto_end == I2C_TRUE)
        /* set auto end */
        i2c->CR2 |= (0x01 << 25);
    else
        /* disable auto end */
        i2c->CR2 &= ~(0x01 << 25);



    /* start tranfer */
    i2c->CR2 |= (0x01 << 13);

    return 0;
}



int i2c_write_byte(uint8_t val, i2c_num_t i2c_num)
{
    I2C_TypeDef *i2c;
    i2c = (I2C_TypeDef*)(I2C1_BASE + (i2c_num*0x400));


    int i = 0;
    /* if out timer out then return -1*/
    /* wait TXIS*/
    while (!((i2c->ISR) & 0x02))
    {
        i++;
        if (i > I2C_TIMEOUT)
            return -1;
    }

    // return -1 if not receive ack
    if ((i2c->ISR >> 4) & 0x01)
    {
        // clear flag
        i2c->ICR |= (1 << 4);
        return -1;
    }

    // write data
    i2c->TXDR = val;

    return 0;
}

int i2c_read_byte(uint8_t *ret, i2c_num_t i2c_num)
{
    I2C_TypeDef *i2c;
    i2c = (I2C_TypeDef*)(I2C1_BASE + (i2c_num*0x400));

    int i = 0;

    /* if out timer out then return -1*/
    while (!((i2c->ISR >> 2) & 0x01))
    {
        i++;
        if (i > I2C_TIMEOUT)
            return -1;
    }

    // return -1 if not receive ack
    if ((i2c->ISR >> 4) & 0x01)
    {
        // clear flag
        i2c->ICR |= (1 << 4);
        return -1;
    }

    *ret = (uint8_t)i2c->RXDR;
    return 0;
}


int i2c_write_reg(uint8_t addr_dev, uint8_t addr, uint8_t val, i2c_num_t num)
{   
    I2C_TypeDef *i2c;
    /*
    if (num == I2C_NUM_4)
        i2c = I2C4;
    else
        i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));
    */
    i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));
    
    /* return -1 if fail in set address */
    // if (i2c_set_address(addr_dev, i2c) == -1)
    //     return -1;


    /* set write mode */
    //I2C_SET_WRITE_MODE(i2c);

    /*  set stop condition */
    // i2c->CR2 |= (0x01 << 25);
    
    /* set byte tranfer */
    // i2c->CR2 &= ~(0xFF << 16); // reset bit
    // i2c->CR2 |= (0x02 << 16); // 2 byte tranfer, 1 for address reg, 1 for val


    /* start tranfer */
    if (i2c_start(num, 2, MODE_WRITE, addr_dev, I2C_TRUE) == -1)
        return -1;

    if (i2c_write_byte(addr, num) == -1 || i2c_write_byte(val, num) == -1)
    {
        i2c_stop_tranfer(num);
        return -1;
    }
    
    // i2c_stop_tranfer(num);
    i2c_clear_flag_stop(i2c);
    return 0;
}

uint8_t i2c_read_reg(uint8_t addr_dev, uint8_t addr, i2c_num_t num)
{
    /* set i2c */
    I2C_TypeDef *i2c;
    uint8_t val;


    /*
    if (num == I2C_NUM_4)
        i2c = I2C4;
    else
        i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));
    */
    i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));

    /* return -1 if fail in set address */
    // if (i2c_set_address(addr_dev, i2c) == -1)
    //     return -1;
    
    /* set byte tranfer */
    // i2c->CR2 &= ~(0xFF << 16); // reset bit
    // i2c->CR2 |= (0x01 << 16); // 1 byte


    /* set write mode */
    // I2C_SET_WRITE_MODE(i2c);

    /* start i2c */
    if (i2c_start(num, 1, MODE_WRITE, addr_dev, I2C_FALSE) == -1)
        return -1;

    /* set address */
    if (i2c_write_byte(addr, num) == -1)
    {
        i2c_stop_tranfer(num);
        return 0;
    }
        

    /* set read mode */
    // I2C_SET_READ_MODE(i2c);


    /*  set stop condition */
    // i2c->CR2 |= (0x01 << 25);

    /* reset byte read */
    // i2c->CR2 &= ~(0xFF << 16); // reset bit
    // i2c->CR2 |= (0x01 << 16); // 1 byte

    /* restart */
    if (i2c_restart(num, 1, MODE_READ, I2C_TRUE) == -1)
    {
        i2c_stop_tranfer(num);
        return -1;
    }
        


    if (i2c_read_byte(&val, num) == -1)
    {
        i2c_stop_tranfer(num);
        return 0;
    }
    
    i2c_clear_flag_stop(i2c);
    // i2c_stop_tranfer(num);
    return val;

}

i2c_bool_t i2c_check_read_burst()
{
    if (FLAG_BURST_READ_DONE)
    {
        FLAG_BURST_READ_DONE = I2C_FALSE;
        return I2C_TRUE;
    }
    return I2C_FALSE;
}

int i2c_burst_read(uint8_t addr_dev, uint8_t addr, uint16_t length, i2c_num_t num, dma_mux1_channel_t dma_channel, uint8_t *ret)
{
    I2C_TypeDef *i2c;

    /*
    if (num == I2C_NUM_4)
        i2c = I2C4;
    else
        i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));
    */
    i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));

    /* return -1 if fail in set address */
    // if (i2c_set_address(addr_dev, i2c) == -1)
    //     return -1;
    
    /* set byte tranfer */
    // i2c->CR2 &= ~(0xFF << 16); // reset bit
    // i2c->CR2 |= (0x01 << 16); // 1 byte


    /* start i2c */
    if (i2c_start(num, 1, MODE_WRITE, addr_dev, I2C_FALSE) == -1)
        return -1;

    /* set address */
    if (i2c_write_byte(addr, num) == -1)
    {
        i2c_stop_tranfer(num);
        return -1;
    }
        

    /* set read mode */
    // I2C_SET_READ_MODE(i2c);

    /* set dma address*/
    dma_SetAddr(&i2c->RXDR, (uint8_t*)ret, length, dma_channel);
    dma_start(dma_channel);
    // flag for check burst read
    FLAG_BURST_READ_DONE = I2C_FALSE;

    /* set byte for read */
    // i2c->CR2 &= ~(0xFF << 16); // reset bit
    // i2c->CR2 |= (length << 16); // 1 byte

    /* set auto end */
    // i2c->CR2 |= (0x01 << 25);

    /* restart */
    if (i2c_restart(num, length, MODE_READ, I2C_TRUE) == -1)
    {
        i2c_stop_tranfer(num);
        return -1;
    }
        
    
    return 0;
}

int i2c_burst_write(uint8_t addr_dev, uint8_t addr, uint16_t length, i2c_num_t num, uint8_t *buff)
{
    I2C_TypeDef *i2c;
    volatile uint16_t i;
    volatile uint16_t j = 0;

    /*
    if (num == I2C_NUM_4)
        i2c = I2C4;
    else
        i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));
    */
    i2c = (I2C_TypeDef*)(I2C1_BASE + (num*0x400));

    /* start i2c */
    // check if length too large to enable reload
    if (length > 255)
    {
        if (i2c_start(num, 256, MODE_WRITE, addr_dev, I2C_TRUE) == -1)
            return -1;
    }
    else 
    {
        if (i2c_start(num, length + 1, MODE_WRITE, addr_dev, I2C_TRUE) == -1)
            return -1;
    }
        


    /* set address */
    if (i2c_write_byte(addr, num) == -1)
    {
        i2c_stop_tranfer(num);
        return -1;
    }

    for (i = 0;i < length;i++)
    {	
		if (j == 255 || i == 254)
        {   
            // slow down cpu
			delay_ms(1);
            // reset j
            j = 0;
        }
            
		// reloade when tranfer complete 255 bytes
        if ((i2c->ISR >> 7) & 0x01)
        {
            // reset nbytes
            i2c->CR2 &= ~(0xFF << 16);
            if (length - i > 255)
                i2c->CR2 |= (0xFF << 16); // set byte tranfer to 255
            else
            {
                i2c->CR2 |= ((length - i) << 16); // set byte tranfer to (length - i)
                // turn off reload bit
                i2c->CR2 &= ~(0x01 << 24);
            }
                
        }
		
        // write byte
        if (i2c_write_byte(buff[i], num) == -1)
        {   
            i2c_stop_tranfer(num);
            // clear all flag
            I2C_CLEAR_ALL_FLAG(i2c);
            return -1;
        }

        // this variable use for slow down cpu when i2c reach 254 bytes tranfer 
        j++;

    }
    // clear flag
    i2c_clear_flag_stop(i2c);
}
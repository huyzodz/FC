#ifndef _VL53L1X_H_
#define _VL53L1X_H_

// power on consume 1.2ms to on


typedef uint8_t VL53L1X_ERROR;


static void VL53L1X_POWER_ON(void);
static void VL53L1X_POWER_OFF(void);

void vl53l1x_init(void);
int8_t vl53l1x_send_cmd();
int8_t vl53l1x_read_data(int16_t *ret);


VL53L1X_ERROR VL53L1X_ClearInterrupt(uint16_t dev);
int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data);
int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data);
int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data);
int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data);

#endif
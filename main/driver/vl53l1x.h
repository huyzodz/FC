#ifndef _VL53L1X_H_
#define _VL53L1X_H_

#include <stdint.h>

// power on consume 1.2ms to on


typedef uint8_t VL53L1X_ERROR;


void VL53L1X_POWER_ON(void);
void VL53L1X_POWER_OFF(void);

void vl53l1x_init(void);
int8_t vl53l1x_send_cmd();
int8_t vl53l1x_read_data(int16_t *ret);


int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data);
int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data);
int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data);
int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data);
int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data);


// use only in this file 

/**
 * @brief This function loads the 135 bytes default values to initialize the sensor.
 * @param dev Device address
 * @return 0:success, != 0:failed
 */
VL53L1X_ERROR VL53L1X_SensorInit(uint16_t dev);

/**
 * @brief This function clears the interrupt, to be called after a ranging data reading
 * to arm the interrupt for the next data ready event.
 */
VL53L1X_ERROR VL53L1X_ClearInterrupt(uint16_t dev);

/**
 * @brief This function programs the interrupt polarity\n
 * 1=active high (default), 0=active low
 */
VL53L1X_ERROR VL53L1X_SetInterruptPolarity(uint16_t dev, uint8_t IntPol);

/**
 * @brief This function returns the current interrupt polarity\n
 * 1=active high (default), 0=active low
 */
VL53L1X_ERROR VL53L1X_GetInterruptPolarity(uint16_t dev, uint8_t *pIntPol);

/**
 * @brief This function starts the ranging distance operation\n
 * The ranging operation is continuous. The clear interrupt has to be done after each get data to allow the interrupt to raise when the next data is ready\n
 * 1=active high (default), 0=active low, use SetInterruptPolarity() to change the interrupt polarity if required.
 */
VL53L1X_ERROR VL53L1X_StartRanging(uint16_t dev);

/**
 * @brief This function stops the ranging.
 */
VL53L1X_ERROR VL53L1X_StopRanging(uint16_t dev);

/**
 * @brief This function checks if the new ranging data is available by polling the dedicated register.
 * @param : isDataReady==0 -> not ready; isDataReady==1 -> ready
 */
VL53L1X_ERROR VL53L1X_CheckForDataReady(uint16_t dev, uint8_t *isDataReady);

/**
 * @brief This function programs the timing budget in ms.
 * Predefined values = 15, 20, 33, 50, 100(default), 200, 500.
 */
VL53L1X_ERROR VL53L1X_SetTimingBudgetInMs(uint16_t dev, uint16_t TimingBudgetInMs);

/**
 * @brief This function returns the current timing budget in ms.
 */
VL53L1X_ERROR VL53L1X_GetTimingBudgetInMs(uint16_t dev, uint16_t *pTimingBudgetInMs);

/**
 * @brief This function programs the distance mode (1=short, 2=long(default)).
 * Short mode max distance is limited to 1.3 m but better ambient immunity.\n
 * Long mode can range up to 4 m in the dark with 200 ms timing budget.
 */
VL53L1X_ERROR VL53L1X_SetDistanceMode(uint16_t dev, uint16_t DistanceMode);

/**
 * @brief This function returns the sensor id, sensor Id must be 0xEACC
 */
VL53L1X_ERROR VL53L1X_GetSensorId(uint16_t dev, uint16_t *id);

/**
 * @brief This function returns the distance measured by the sensor in mm
 */
VL53L1X_ERROR VL53L1X_GetDistance(uint16_t dev, uint16_t *distance);

/**
 * @brief This function programs the Intermeasurement period in ms\n
 * Intermeasurement period must be >/= timing budget. This condition is not checked by the API,
 * the customer has the duty to check the condition. Default = 100 ms
 */
VL53L1X_ERROR VL53L1X_SetInterMeasurementInMs(uint16_t dev,
					 uint32_t InterMeasurementInMs);

/**
 * @brief This function returns the current distance mode (1=short, 2=long).
 */
VL53L1X_ERROR VL53L1X_GetDistanceMode(uint16_t dev, uint16_t *pDistanceMode);

/**
 * @brief This function programs the ROI (Region of Interest)\n
 * The ROI position is centered, only the ROI size can be reprogrammed.\n
 * The smallest acceptable ROI size = 4\n
 * @param X:ROI Width; Y=ROI Height
 */
VL53L1X_ERROR VL53L1X_SetROI(uint16_t dev, uint16_t X, uint16_t Y);

/**
 * @brief This function returns the ranging status error \n
 * (0:no error, 1:sigma failed, 2:signal failed, ..., 7:wrap-around)
 */
VL53L1X_ERROR VL53L1X_GetRangeStatus(uint16_t dev, uint8_t *rangeStatus);

#endif
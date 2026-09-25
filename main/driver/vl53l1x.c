#include "vl53l1x.h"
#include "i2c_master.h"
#include "gpio.h"
#include "timer.h"


#define ADDRESS_DEVICE_VL53L1X_READ     0x53          
#define ADDRESS_DEVICE_VL53L1X_WRITE    0x52
#define ADDRESS_READ_DATA_VL53L1X       

#define CHANNEL_DMA_READ_VL53L1X        DMA_MUX_CHANNEL_8

#define GPIO_SCL_VL53L1X                8
#define GPIO_SDA_VL53L1X                9
#define GPIO_PORT_SCL_VL53L1X           GPIO_PORT_A
#define GPIO_PORT_SDA_VL53L1X           GPIO_PORT_C
#define NUM_I2C_VL53L1X                 I2C_NUM_3

#define MODE_I2C_SPEED_VL53L1X          I2C_MODE_400KHZ

#define GPIO_XSHUT_VL53L1X              8
#define GPIO_PORT_XSHUT_VL53L1X         GPIO_PORT_C


#define VL53L1X_SIZE_READ                           4




#define VL53L1X_IMPLEMENTATION_VER_MAJOR       3
#define VL53L1X_IMPLEMENTATION_VER_MINOR       5
#define VL53L1X_IMPLEMENTATION_VER_SUB         5
#define VL53L1X_IMPLEMENTATION_VER_REVISION  0000

#define VL53L1X_ERROR_NONE					((uint8_t)0U)
#define VL53L1X_ERROR_TIMEOUT				((uint8_t)255U)

#define SOFT_RESET											0x0000
#define VL53L1_I2C_SLAVE__DEVICE_ADDRESS					0x0001
#define VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND        0x0008
#define ALGO__CROSSTALK_COMPENSATION_PLANE_OFFSET_KCPS 		0x0016
#define ALGO__CROSSTALK_COMPENSATION_X_PLANE_GRADIENT_KCPS 	0x0018
#define ALGO__CROSSTALK_COMPENSATION_Y_PLANE_GRADIENT_KCPS 	0x001A
#define ALGO__PART_TO_PART_RANGE_OFFSET_MM					0x001E
#define MM_CONFIG__INNER_OFFSET_MM							0x0020
#define MM_CONFIG__OUTER_OFFSET_MM 							0x0022
#define GPIO_HV_MUX__CTRL									0x0030
#define GPIO__TIO_HV_STATUS       							0x0031
#define SYSTEM__INTERRUPT_CONFIG_GPIO 						0x0046
#define PHASECAL_CONFIG__TIMEOUT_MACROP     				0x004B
#define RANGE_CONFIG__TIMEOUT_MACROP_A_HI   				0x005E
#define RANGE_CONFIG__VCSEL_PERIOD_A        				0x0060
#define RANGE_CONFIG__VCSEL_PERIOD_B						0x0063
#define RANGE_CONFIG__TIMEOUT_MACROP_B_HI  					0x0061
#define RANGE_CONFIG__TIMEOUT_MACROP_B_LO  					0x0062
#define RANGE_CONFIG__SIGMA_THRESH 							0x0064
#define RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS			0x0066
#define RANGE_CONFIG__VALID_PHASE_HIGH      				0x0069
#define VL53L1_SYSTEM__INTERMEASUREMENT_PERIOD				0x006C
#define SYSTEM__THRESH_HIGH 								0x0072
#define SYSTEM__THRESH_LOW 									0x0074
#define SD_CONFIG__WOI_SD0                  				0x0078
#define SD_CONFIG__INITIAL_PHASE_SD0        				0x007A
#define ROI_CONFIG__USER_ROI_CENTRE_SPAD					0x007F
#define ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE		0x0080
#define SYSTEM__SEQUENCE_CONFIG								0x0081
#define VL53L1_SYSTEM__GROUPED_PARAMETER_HOLD 				0x0082
#define SYSTEM__INTERRUPT_CLEAR       						0x0086
#define SYSTEM__MODE_START                 					0x0087
#define VL53L1_RESULT__RANGE_STATUS							0x0089
#define VL53L1_RESULT__DSS_ACTUAL_EFFECTIVE_SPADS_SD0		0x008C
#define RESULT__AMBIENT_COUNT_RATE_MCPS_SD					0x0090
#define VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0				0x0096
#define VL53L1_RESULT__PEAK_SIGNAL_COUNT_RATE_CROSSTALK_CORRECTED_MCPS_SD0 	0x0098
#define VL53L1_RESULT__OSC_CALIBRATE_VAL					0x00DE
#define VL53L1_FIRMWARE__SYSTEM_STATUS                      0x00E5
#define VL53L1_IDENTIFICATION__MODEL_ID                     0x010F
#define VL53L1_ROI_CONFIG__MODE_ROI_CENTRE_SPAD				0x013E


// do not touch
const uint8_t VL51L1X_DEFAULT_CONFIGURATION[] = {
    0x00, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C), else don't touch */
    0x00, /* 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
    0x00, /* 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
    0x01, /* 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low (bits 3:0 must be 0x1), use SetInterruptPolarity() */
    0x02, /* 0x31 : bit 1 = interrupt depending on the polarity, use CheckForDataReady() */
    0x00, /* 0x32 : not user-modifiable */
    0x02, /* 0x33 : not user-modifiable */
    0x08, /* 0x34 : not user-modifiable */
    0x00, /* 0x35 : not user-modifiable */
    0x08, /* 0x36 : not user-modifiable */
    0x10, /* 0x37 : not user-modifiable */
    0x01, /* 0x38 : not user-modifiable */
    0x01, /* 0x39 : not user-modifiable */
    0x00, /* 0x3a : not user-modifiable */
    0x00, /* 0x3b : not user-modifiable */
    0x00, /* 0x3c : not user-modifiable */
    0x00, /* 0x3d : not user-modifiable */
    0xff, /* 0x3e : not user-modifiable */
    0x00, /* 0x3f : not user-modifiable */
    0x0F, /* 0x40 : not user-modifiable */
    0x00, /* 0x41 : not user-modifiable */
    0x00, /* 0x42 : not user-modifiable */
    0x00, /* 0x43 : not user-modifiable */
    0x00, /* 0x44 : not user-modifiable */
    0x00, /* 0x45 : not user-modifiable */
    0x20, /* 0x46 : interrupt configuration 0->level low detection, 1-> level high, 2-> Out of window, 3->In window, 0x20-> New sample ready , TBC */
    0x0b, /* 0x47 : not user-modifiable */
    0x00, /* 0x48 : not user-modifiable */
    0x00, /* 0x49 : not user-modifiable */
    0x02, /* 0x4a : not user-modifiable */
    0x0a, /* 0x4b : not user-modifiable */
    0x21, /* 0x4c : not user-modifiable */
    0x00, /* 0x4d : not user-modifiable */
    0x00, /* 0x4e : not user-modifiable */
    0x05, /* 0x4f : not user-modifiable */
    0x00, /* 0x50 : not user-modifiable */
    0x00, /* 0x51 : not user-modifiable */
    0x00, /* 0x52 : not user-modifiable */
    0x00, /* 0x53 : not user-modifiable */
    0xc8, /* 0x54 : not user-modifiable */
    0x00, /* 0x55 : not user-modifiable */
    0x00, /* 0x56 : not user-modifiable */
    0x38, /* 0x57 : not user-modifiable */
    0xff, /* 0x58 : not user-modifiable */
    0x01, /* 0x59 : not user-modifiable */
    0x00, /* 0x5a : not user-modifiable */
    0x08, /* 0x5b : not user-modifiable */
    0x00, /* 0x5c : not user-modifiable */
    0x00, /* 0x5d : not user-modifiable */
    0x01, /* 0x5e : not user-modifiable */
    0xcc, /* 0x5f : not user-modifiable */
    0x0f, /* 0x60 : not user-modifiable */
    0x01, /* 0x61 : not user-modifiable */
    0xf1, /* 0x62 : not user-modifiable */
    0x0d, /* 0x63 : not user-modifiable */
    0x01, /* 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB), use SetSigmaThreshold(), default value 90 mm  */
    0x68, /* 0x65 : Sigma threshold LSB */
    0x00, /* 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB), use SetSignalThreshold() */
    0x80, /* 0x67 : Min count Rate LSB */
    0x08, /* 0x68 : not user-modifiable */
    0xb8, /* 0x69 : not user-modifiable */
    0x00, /* 0x6a : not user-modifiable */
    0x00, /* 0x6b : not user-modifiable */
    0x00, /* 0x6c : Intermeasurement period MSB, 32 bits register, use SetIntermeasurementInMs() */
    0x00, /* 0x6d : Intermeasurement period */
    0x0f, /* 0x6e : Intermeasurement period */
    0x89, /* 0x6f : Intermeasurement period LSB */
    0x00, /* 0x70 : not user-modifiable */
    0x00, /* 0x71 : not user-modifiable */
    0x00, /* 0x72 : distance threshold high MSB (in mm, MSB+LSB), use SetD:tanceThreshold() */
    0x00, /* 0x73 : distance threshold high LSB */
    0x00, /* 0x74 : distance threshold low MSB ( in mm, MSB+LSB), use SetD:tanceThreshold() */
    0x00, /* 0x75 : distance threshold low LSB */
    0x00, /* 0x76 : not user-modifiable */
    0x01, /* 0x77 : not user-modifiable */
    0x0f, /* 0x78 : not user-modifiable */
    0x0d, /* 0x79 : not user-modifiable */
    0x0e, /* 0x7a : not user-modifiable */
    0x0e, /* 0x7b : not user-modifiable */
    0x00, /* 0x7c : not user-modifiable */
    0x00, /* 0x7d : not user-modifiable */
    0x02, /* 0x7e : not user-modifiable */
    0xc7, /* 0x7f : ROI center, use SetROI() */
    0xff, /* 0x80 : XY ROI (X=Width, Y=Height), use SetROI() */
    0x9B, /* 0x81 : not user-modifiable */
    0x00, /* 0x82 : not user-modifiable */
    0x00, /* 0x83 : not user-modifiable */
    0x00, /* 0x84 : not user-modifiable */
    0x01, /* 0x85 : not user-modifiable */
    0x00, /* 0x86 : clear interrupt, use ClearInterrupt() */
    0x00  /* 0x87 : start ranging, use StartRanging() or StopRanging(), If you want an automatic start after VL53L1X_init() call, put 0x40 in location 0x87 */
};


VL53L1X_ERROR VL53L1X_StartRanging(uint16_t dev)
{
	VL53L1X_ERROR status = 0;

	status |= VL53L1_WrByte(dev, SYSTEM__MODE_START, 0x40);	/* Enable VL53L1X */
	return status;
}

VL53L1X_ERROR VL53L1X_StopRanging(uint16_t dev)
{
	VL53L1X_ERROR status = 0;

	status |= VL53L1_WrByte(dev, SYSTEM__MODE_START, 0x00);	/* Disable VL53L1X */
	return status;
}

VL53L1X_ERROR VL53L1X_GetInterruptPolarity(uint16_t dev, uint8_t *pInterruptPolarity)
{
	uint8_t Temp;
	VL53L1X_ERROR status = 0;

	status |= VL53L1_RdByte(dev, GPIO_HV_MUX__CTRL, &Temp);
	Temp = Temp & 0x10;
	*pInterruptPolarity = !(Temp>>4);
	return status;
}

VL53L1X_ERROR VL53L1X_SetInterruptPolarity(uint16_t dev, uint8_t NewPolarity)
{
	uint8_t Temp;
	VL53L1X_ERROR status = 0;

	status |= VL53L1_RdByte(dev, GPIO_HV_MUX__CTRL, &Temp);
	Temp = Temp & 0xEF;
	status |= VL53L1_WrByte(dev, GPIO_HV_MUX__CTRL, Temp | (!(NewPolarity & 1)) << 4);
	return status;
}

VL53L1X_ERROR VL53L1X_CheckForDataReady(uint16_t dev, uint8_t *isDataReady)
{
	uint8_t Temp;
	uint8_t IntPol;
	VL53L1X_ERROR status = 0;

	status |= VL53L1X_GetInterruptPolarity(dev, &IntPol);
	status |= VL53L1_RdByte(dev, GPIO__TIO_HV_STATUS, &Temp);
	/* Read in the register to check if a new value is available */
	if (status == 0){
		if ((Temp & 1) == IntPol)
			*isDataReady = 1;
		else
			*isDataReady = 0;
	}
	return status;
}


VL53L1X_ERROR VL53L1X_GetTimingBudgetInMs(uint16_t dev, uint16_t *pTimingBudget)
{
	uint16_t Temp;
	VL53L1X_ERROR status = 0;

	status |= VL53L1_RdWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI, &Temp);
	switch (Temp) {
		case 0x001D :
			*pTimingBudget = 15;
			break;
		case 0x0051 :
		case 0x001E :
			*pTimingBudget = 20;
			break;
		case 0x00D6 :
		case 0x0060 :
			*pTimingBudget = 33;
			break;
		case 0x1AE :
		case 0x00AD :
			*pTimingBudget = 50;
			break;
		case 0x02E1 :
		case 0x01CC :
			*pTimingBudget = 100;
			break;
		case 0x03E1 :
		case 0x02D9 :
			*pTimingBudget = 200;
			break;
		case 0x0591 :
		case 0x048F :
			*pTimingBudget = 500;
			break;
		default:
			status = 1;
			*pTimingBudget = 0;
	}
	return status;
}

VL53L1X_ERROR VL53L1X_SetTimingBudgetInMs(uint16_t dev, uint16_t TimingBudgetInMs)
{
	uint16_t DM;
	VL53L1X_ERROR  status=0;

	status |= VL53L1X_GetDistanceMode(dev, &DM);
	if (DM == 0)
		return 1;
	else if (DM == 1) {	/* Short DistanceMode */
		switch (TimingBudgetInMs) {
		case 15: /* only available in short distance mode */
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x01D);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0027);
			break;
		case 20:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x0051);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x006E);
			break;
		case 33:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x00D6);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x006E);
			break;
		case 50:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x1AE);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x01E8);
			break;
		case 100:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x02E1);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0388);
			break;
		case 200:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x03E1);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0496);
			break;
		case 500:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x0591);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x05C1);
			break;
		default:
			status = 1;
			break;
		}
	} else {
		switch (TimingBudgetInMs) {
		case 20:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x001E);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0022);
			break;
		case 33:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x0060);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x006E);
			break;
		case 50:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x00AD);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x00C6);
			break;
		case 100:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x01CC);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x01EA);
			break;
		case 200:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x02D9);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x02F8);
			break;
		case 500:
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x048F);
			VL53L1_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x04A4);
			break;
		default:
			status = 1;
			break;
		}
	}
	return status;
}

VL53L1X_ERROR VL53L1X_CheckForDataReady(uint16_t dev, uint8_t *isDataReady)
{
	uint8_t Temp;
	uint8_t IntPol;
	VL53L1X_ERROR status = 0;

	status |= VL53L1X_GetInterruptPolarity(dev, &IntPol);
	status |= VL53L1_RdByte(dev, GPIO__TIO_HV_STATUS, &Temp);
	/* Read in the register to check if a new value is available */
	if (status == 0){
		if ((Temp & 1) == IntPol)
			*isDataReady = 1;
		else
			*isDataReady = 0;
	}
	return status;
}

VL53L1X_ERROR VL53L1X_SetDistanceMode(uint16_t dev, uint16_t DM)
{
	uint16_t TB;
	VL53L1X_ERROR status = 0;

	status |= VL53L1X_GetTimingBudgetInMs(dev, &TB);
	if (status != 0)
		return 1;
	switch (DM) {
	case 1:
		status = VL53L1_WrByte(dev, PHASECAL_CONFIG__TIMEOUT_MACROP, 0x14);
		status = VL53L1_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_A, 0x07);
		status = VL53L1_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_B, 0x05);
		status = VL53L1_WrByte(dev, RANGE_CONFIG__VALID_PHASE_HIGH, 0x38);
		status = VL53L1_WrWord(dev, SD_CONFIG__WOI_SD0, 0x0705);
		status = VL53L1_WrWord(dev, SD_CONFIG__INITIAL_PHASE_SD0, 0x0606);
		break;
	case 2:
		status = VL53L1_WrByte(dev, PHASECAL_CONFIG__TIMEOUT_MACROP, 0x0A);
		status = VL53L1_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_A, 0x0F);
		status = VL53L1_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_B, 0x0D);
		status = VL53L1_WrByte(dev, RANGE_CONFIG__VALID_PHASE_HIGH, 0xB8);
		status = VL53L1_WrWord(dev, SD_CONFIG__WOI_SD0, 0x0F0D);
		status = VL53L1_WrWord(dev, SD_CONFIG__INITIAL_PHASE_SD0, 0x0E0E);
		break;
	default:
		status = 1;
		break;
	}

	if (status == 0)
		status |= VL53L1X_SetTimingBudgetInMs(dev, TB);
	return status;
}

VL53L1X_ERROR VL53L1X_StartRanging(uint16_t dev)
{
	VL53L1X_ERROR status = 0;

	status |= VL53L1_WrByte(dev, SYSTEM__MODE_START, 0x40);	/* Enable VL53L1X */
	return status;
}

VL53L1X_ERROR VL53L1X_SensorInit(uint16_t dev)
{
	VL53L1X_ERROR status = 0;
	uint8_t Addr = 0x00, tmp =0;
	uint16_t timeout_counter = 0;

	for (Addr = 0x2D; Addr <= 0x87; Addr++){
		status |= VL53L1_WrByte(dev, Addr, VL51L1X_DEFAULT_CONFIGURATION[Addr - 0x2D]);
	}
	status |= VL53L1X_StartRanging(dev);
	while (tmp == 0)
	{
		status = VL53L1X_CheckForDataReady(dev, &tmp);
		timeout_counter++;
		if (timeout_counter >= 1000)
		{
			status = (uint8_t)VL53L1X_ERROR_TIMEOUT;
			return status;
		}
		status = VL53L1_WaitMs(dev, 1);
	}
	status |= VL53L1X_ClearInterrupt(dev);
	status |= VL53L1X_StopRanging(dev);
	status |= VL53L1_WrByte(dev, VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 0x09); /* two bounds VHV */
	status |= VL53L1_WrByte(dev, 0x0B, 0); /* start VHV from the previous temperature */
	return status;
}

VL53L1X_ERROR VL53L1X_GetDistance(uint16_t dev, uint16_t *distance)
{
	VL53L1X_ERROR status = 0;
	uint16_t tmp;

	status |= (VL53L1_RdWord(dev,
			VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0, &tmp));

	*distance = tmp;
	return status;
}


/*              touch here          */

volatile uint8_t vl53l1x_data_ram [VL53L1X_SIZE_READ * 2];
int8_t flag_read_vl53l1x = 0;
int8_t flag_read_vl53l1x_err = 0;
static uint16_t distance_base = 0;


void vl53l1x_init(void)
{
    uint16_t dev = 0x52;
    /* init i2c */
    i2c_config_t cfg = {
        .analog_filter = I2C_TRUE,
        .dma_read = CHANNEL_DMA_READ_VL53L1X,
        .gpio_scl = GPIO_SCL_VL53L1X,
        .gpio_sda = GPIO_SDA_VL53L1X,
        .i2c_mode = MODE_I2C_SPEED_VL53L1X,
        .i2c_num = NUM_I2C_VL53L1X,
        .irq_en = I2C_FALSE,
        .port_scl = GPIO_PORT_SCL_VL53L1X,
        .port_sda = GPIO_PORT_SDA_VL53L1X
    };
    i2c_init(cfg);


    // init control xshut
    gpio_config_t gpio_cfg = {
        .alternate = AF0,
        .gpio = GPIO_PORT_XSHUT_VL53L1X,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = low_speed,
        .pinNum = GPIO_XSHUT_VL53L1X,
        .pull = reserved
    };
    gpio_init(&gpio_cfg);   

    // write 1 to xshut
    VL53L1X_POWER_ON();
    delay_ms(100);

    // init sensor
    while (1)
    {
        /* This function must to be called to initialize the sensor with the default setting  */
        if (VL53L1X_SensorInit(dev) != 0)
            continue;
        /* Optional functions to be used to change the main ranging parameters according the application requirements to get the best ranging performances */
        if (VL53L1X_SetDistanceMode(dev, 2) != 0) /* 1=short, 2=long */
            continue;
        if (VL53L1X_SetTimingBudgetInMs(dev, 100) != 0) /* in ms possible values [20, 50, 100, 200, 500] */
            continue;
        if (VL53L1X_SetInterMeasurementInMs(dev, 100) != 0) /* in ms, IM must be > = TB */
            continue;
        if (VL53L1X_SetInterruptPolarity(dev,0) != 0) //This function programs the interrupt polarity, 1 = active high (default), 0 = active low.
            continue;

        break;
    }

    // start
    VL53L1X_StartRanging(dev);
    delay_ms(10);

    // calculate offset value when in grown
    uint16_t sum = 0;
    int i = 0;
    while (i < 10)
    {
        uint16_t temp;
        if (VL53L1X_GetDistance(dev, &temp) != 0)
            continue;
        VL53L1X_ClearInterrupt(dev);
        i++;
        sum += temp;
        delay_ms(100);
    }
    distance_base = sum/10;
}



/*                      USE GLB */

int8_t vl53l1x_send_cmd()
{
    int check;
    uint8_t *ptr;
    if (flag_read_vl53l1x == 0)
        ptr = vl53l1x_data_ram;
    else
        ptr = vl53l1x_data_ram + VL53L1X_SIZE_READ;
    
    check = i2c_burst_read_addr_16bit(ADDRESS_DEVICE_VL53L1X_READ, VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0, VL53L1X_SIZE_READ, NUM_I2C_VL53L1X, CHANNEL_DMA_READ_VL53L1X, ptr);

    if (check == 0)
    {
        // handle err for read
        flag_read_vl53l1x = (flag_read_vl53l1x == 0) ? 1 : 0;
        flag_read_vl53l1x_err = 0;
    }
    else
    {
        flag_read_vl53l1x_err = 1;
        return -1;
    }
    return check;
}

int8_t vl53l1x_read_data(int16_t *ret)
{
	int8_t status = 0;
    uint8_t *temp;
    uint16_t temp_data;

	if (i2c_check_read_burst(NUM_I2C_VL53L1X) != I2C_TRUE)
        return -1;

	if (flag_read_vl53l1x_err)
        return -1;

    if (flag_read_vl53l1x == 0)
        temp = vl53l1x_data_ram + VL53L1X_SIZE_READ;
    else
        temp = vl53l1x_data_ram;
    
    temp_data = (uint16_t)(temp[0] << 8) | temp[1];

    *ret = (int16_t)(temp_data - distance_base);
    // clean irq before send
    VL53L1X_ClearInterrupt(ADDRESS_DEVICE_VL53L1X_WRITE);

	return status;
}

VL53L1X_ERROR VL53L1X_ClearInterrupt(uint16_t dev)
{
	VL53L1X_ERROR status = 0;

	status |= VL53L1_WrByte(dev, SYSTEM__INTERRUPT_CLEAR, 0x01);
	return status;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) 
{
	uint8_t status = -1;
    uint8_t data_send [] = {(uint8_t)((index >> 8) & 0x00FF), (uint8_t)(index & 0x00FF), data};
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
    // prepare data

	status = i2c_send((uint8_t)dev, data_send, 3, NUM_I2C_VL53L1X);

	return status;
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) 
{
	uint8_t status = 255;
	uint8_t data_send [] = {(uint8_t)((index >> 8) & 0x00FF), (uint8_t)(index & 0x00FF), (uint8_t)((data >> 8) & 0x00FF), (uint8_t)(data & 0x00FF)};
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	status = i2c_send((uint8_t)dev, data_send, 4, NUM_I2C_VL53L1X);

	return status;
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) 
{
	uint8_t status = 255;
    dev |= 0x01;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
    status = i2c_recieve((uint8_t)dev, index, data, NUM_I2C_VL53L1X);

	return status;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) 
{
	uint8_t status = 255;
    uint8_t data_rx [2];
    dev |= 0x01;
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
    status = i2c_burst_read_addr_16bit(dev, index, 2, NUM_I2C_VL53L1X, CHANNEL_DMA_READ_VL53L1X, data_rx);
	
    // wait for read done
    while (i2c_check_read_burst(NUM_I2C_VL53L1X) != I2C_FALSE);

    *data = (uint16_t)(data_rx[0] << 8) | data_rx[1];

	return status;
}


static void VL53L1X_POWER_ON(void)
{
    gpio_write(GPIO_PORT_XSHUT_VL53L1X, GPIO_XSHUT_VL53L1X, 1);
}

static void VL53L1X_POWER_OFF(void)
{
    gpio_write(GPIO_PORT_XSHUT_VL53L1X, GPIO_XSHUT_VL53L1X, 0);
}
#include "init.h"

#include "clock.h"
#include "timer.h"
#include "dshot.h"
#include "imu_bmi270.h"
#include "rasp_com.h"
#include "drone.h"


void phase_1_init(void)
{
    // init clock source
    clock_init(CLOCK_240_MHZ);

    // init delay
    timer_init();

    // dshot init
    dshot_handle_t dshot = {
        .dshot_type = DShot600,
        .gpio_port = GPIO_PORT_A,
        .pin_start = 0
    };
    dshot_init(dshot);
    delay_ms(100);
}

void phase_2_init(void)
{
    // init bmi270
    bmi270_init(BMI270_NORMAL_POWER_MODE);
	delay_ms(10);
    // init rasp_com
    rasp_com_init();
	delay_ms(100);

}

void phase_3_init(void)
{
    // layer 1 roll pitch and z yaw
    // pid
    // need to config or change tunning
    // pitch
    pid_config_t cfg = {
        .kp = 0.4f,
        .ki = 0.05f,
        .kd = 0,
        .max_derivative = 0,
        .min_derivative = 0,
        .max_intergral = 2.0f,
        .min_intergral = -2.0f,
        .max_out = 1.0f,
        .min_out = -1.0f
    };
    pid_init(&drone_pitch, cfg);

    // roll
    cfg.kd = 0.2f;
    pid_init(&drone_roll, cfg);

    // z
    cfg = (pid_config_t) {
        .kp = 2.0f,
        .ki = 0.5f,
        .kd = 0.4f,
        .max_derivative = 10.0f,
        .min_derivative = -10.0f,
        .max_intergral = 10.0f,
        .min_intergral = -10.0f,
        .max_out = 2.0f,
        .min_out = -2.0f
    };
    pid_init(&drone_z, cfg);

    // layer 2 smc for roll pitch z
    // pitch
    smc_init(&drone_rate_pitch, 0.05f, (1.1*0.05f), -10.0f, 10.0f);

    // roll
    smc_init(&drone_rate_roll, 0.05f, (1.1*0.05f), -10.0f, 10.0f);

    // z
    smc_init(&drone_rate_z, 2.0f, (1.1*2.0f), -10.0f, 10.0f);






    // yaw layer 1 and 2
    // layer 1~
    cfg.kp = 0.004f;
    cfg.ki = 0;
    cfg.kd = 0;
    cfg.max_out = 5.0f;
    cfg.min_out = -5.0f;
    pid_init(&drone_yaw, cfg);
    // layer 2
    cfg.kp = 0.3f*0.004f;
    cfg.max_out = 2.0f;
    cfg.min_out = -2.0f;
    pid_init(&drone_rate_yaw, cfg);




    delay_ms(100);
}


void system_init(void)
{
    // first init for system just like clock, timer...
    phase_1_init();

    // second init this init for driver and sensor and some peripheral..
    phase_2_init();

    // third init this init for controller system
    phase_3_init();

}
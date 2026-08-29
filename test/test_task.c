#include "test.h"
#include "drone.h"
#include "task.h"
#include "timer.h"
#include "drone_scheduler.h"


uint32_t test_time;

void test_task_imu(void)
{
    uint8_t task_num = 2;
    bmi270_calib();

    
    init_task(&TASK_DRONE[task_num]);
    START_SYSTEM_TIMER();
    while (1)
    {
        // measure task take time
        TASK_DRONE[task_num].run(&TASK_DRONE[task_num]);
        test_time = TASK_DRONE[task_num].excution_time_last_run;
        delay_ms(100);
    }
}
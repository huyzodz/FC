#include "drone_scheduler.h"
#include "timer.h"
#include "task.h"
#include <stddef.h>

// us
#define OVERTIME_TASK_WAIT                  2

static inline int8_t CHECK_READY_2_RUN(const task_t *task, task_t *next_important_task)
{
    uint32_t temp_time = GET_CURRENT_US();
    if (temp_time < (task->expect_next_time_run - OVERTIME_TASK_WAIT))
    {
        // not time to run
        return -1;
    } 
    else 
    {
        // check next time is task more important or not
        //if ((next_important_task->expect_next_time_run - OVERTIME_TASK_WAIT) < )
        if (1)
        {
            if (temp_time >= (task->expect_next_time_run - OVERTIME_TASK_WAIT) && temp_time < task->expect_next_time_run)
                return 1; // let scheduler know to wait
        }
        // ready to run
        return 0;
    }
}

static inline void general_run(task_t *ptr)
{
    // calculate start time
    uint32_t start_time = GET_CURRENT_US();
    // calculate dt
    float dt = CALCULATE_DT(start_time, (*ptr));

    task_data_t data = {
        .dt = dt
    };

    ptr->task(&data);

    update_task(&(*ptr), start_time);
}


// need add more if need
inline void update_task(task_t *ret, uint32_t start_time)
{
    if (ret->priority != TASK_REAL_TIME)
    {
        // handle background task here


    }
    ret->last_start_time = start_time;
    // expeted next time to run
    ret->expect_next_time_run += ret->period; // us
    ret->last_stop_time = GET_CURRENT_US();
    ret->excution_time_last_run = ret->last_stop_time - start_time;
}


void init_task(task_t *ret)
{
    ret->run = &general_run;
    ret->period = ((uint32_t)(1000000UL / ret->freq));
}



//                     ***************** FLIGHT HERE
void FLIGHT_SYSTEM(void)
{
    task_t *next_important_task = NULL;
    while (1)
    {
        // run task here
        for (int i = 0;i < TASK_LENGTH;i++)
        {
            // ready to run
            int8_t check = CHECK_READY_2_RUN(&TASK_DRONE[i], next_important_task);
            if (check == 0 || check == 1)
            {
                if (check == 1)
                {
                    // wait until ready to run
                    while (CHECK_READY_2_RUN(&TASK_DRONE[i], next_important_task) != 0);
                }
                
                // run task
                TASK_DRONE[i].run(&TASK_DRONE[i]);
                // clear if current task is important task
                if (next_important_task == &TASK_DRONE[i])
                    next_important_task = NULL;

                // update next_important_task
            }
            // else not run and check other task
        }
    }
}




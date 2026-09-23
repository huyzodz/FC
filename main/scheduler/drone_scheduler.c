#include "drone_scheduler.h"
#include "timer.h"
#include "task.h"
#include <stddef.h>

#ifdef SIMULATION_ON
task_t *next_important_task = NULL;
#endif

// us
#define OVERTIME_TASK_WAIT                  0

// return
// < 0 if a < b
static inline int32_t compare(uint32_t a, uint32_t b)
{
    return (int32_t)(a - b);
}

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
        if (next_important_task != NULL && next_important_task != task)
        {
            if ((next_important_task->expect_next_time_run - OVERTIME_TASK_WAIT) < temp_time + task->excution_time_last_run)
                // not run current task
                return -1;
        }
            
        if (temp_time >= (task->expect_next_time_run - OVERTIME_TASK_WAIT) && temp_time < task->expect_next_time_run)
            return 1; // let scheduler know to wait

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
    
    ptr->dt = dt;

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
    ret->last_stop_time = GET_CURRENT_US();

    // ret->expect_next_time_run += ret->period; // us
    // handle if late for net period too much
    ret->expect_next_time_run += (ret->period * (ret->last_stop_time/ret->expect_next_time_run + 1));


    ret->excution_time_last_run = ret->last_stop_time - start_time;
}


void init_task(task_t *ret)
{
    ret->run = &general_run;
    ret->period = ((uint32_t)(1000000UL / ret->freq));
}



task_t *handle_important_task(task_t *current_important_task, int i)
{
    // update next_important_task
    if (current_important_task == NULL)
        current_important_task = &TASK_DRONE[i];
    else
    {
        // check if current important task next run time, sooner than current checking task
        if (current_important_task->expect_next_time_run < TASK_DRONE[i].expect_next_time_run)
        {
            // check priority
            // smaller => more important
            if (current_important_task->priority > TASK_DRONE[i].priority)
            {
                // if this current important task priority is smaller than checking task
                // check if current important task last excution 
                if (current_important_task->excution_time_last_run + current_important_task->expect_next_time_run > TASK_DRONE[i].expect_next_time_run)
                {
                    // if next time run of current important task + excution task take > current checking task => switch important ptr
                    current_important_task = &TASK_DRONE[i];
                }
            }
        }
        else
        {
            // if next time run later current checking
            // check priority
            if (current_important_task->priority > TASK_DRONE[i].priority)
            {
                // if prioroity current important task smaller then switch
                current_important_task = &TASK_DRONE[i];
            }
        }
    }
    return current_important_task;
}


//                     ***************** FLIGHT HERE
void FLIGHT_SYSTEM(void)
{
#ifndef SIMULATION_ON
    task_t *next_important_task = NULL;
#endif
    //init_task(&TASK_DRONE[task_num]);
    START_SYSTEM_TIMER();

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
            }
            else
                next_important_task = handle_important_task(next_important_task, i);
            
            // else not run and check other task
        }
    }
}




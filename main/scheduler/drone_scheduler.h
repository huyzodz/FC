#ifndef _DRONE_SCHEDULER_H_
#define _DRONE_SCHEDULER_H_

#include <stdint.h>

#define CALCULATE_DT(time, task)            ((float)(time - (task.last_start_time))/1e6)


typedef enum {
    TASK_REAL_TIME = -1,

} task_priority_t;


typedef struct task_t task_t;
typedef struct task_data_t task_data_t;

typedef void (*task_function_t)(task_data_t*);  

struct task_data_t {
    float dt;
};


// all time use us
struct task_t{
    // this variable use to check last time task take to run, 
    // this will help desire whether it enough time to run realtime task or not
    uint32_t excution_time_last_run;

    uint32_t expect_next_time_run;
    uint32_t period;

    // config in init
    uint32_t last_stop_time;
    uint32_t last_start_time;

    // this variable will init time to run
    uint32_t init_time_run;
    uint16_t freq;
    // pointer to task to run
    task_function_t task;
    // pointer to task to general function
    // no need to init in hand
    void (*run)(task_t *);
    char *name_task;
    int8_t priority;
};


void init_task(task_t *ret);

inline void update_task(task_t *ret, uint32_t start_time);


void FLIGHT_SYSTEM(void);


#endif
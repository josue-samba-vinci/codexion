#include "codexion.h"

long    get_time_ms(void)
{
    struct  timeval time;
    
    gettimeofday(&time, NULL);
    return ((time.tv_sec*1000)+(time.tv_usec/1000));
}

int sim_is_over(t_config *config)
{
    int actual_end;

    pthread_mutex_lock(&config->state_lock);
    actual_end = config->end;
    pthread_mutex_unlock(&config->state_lock);
    return (actual_end);
}

void    precise_sleep(long duration_ms, t_config *config)
{
    long    start;

    start = get_time_ms();
    while (get_time_ms() - start < duration_ms)
    {
        if (sim_is_over(config))
            return;
        usleep(100);   
    }
}
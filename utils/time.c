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


#include "codexion.h"

int sim_is_over(t_config *config)
{
    int actual_end;

    pthread_mutex_lock(&config->state_lock);
    actual_end = config->end;
    pthread_mutex_unlock(&config->state_lock);
    return (actual_end);
}

void log_action(t_coder *coder, char *msg)
{
    pthread_mutex_lock(&coder->config->print_lock);
    if (!sim_is_over(coder->config))
        printf("%ld %d %s\n", get_time_ms() - coder->config->start, coder->id, msg);
    pthread_mutex_unlock(&coder->config->print_lock);
}
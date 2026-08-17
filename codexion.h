#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <limits.h>
# include <stddef.h>
# include <stdio.h>

typedef struct s_config
{
    int     nb_coders;
    long    time_to_burnout;
    long    time_to_compile;
    long    time_to_debug;
    long    time_to_refactor;
    int     nb_compiles_required;
    long    dongle_cooldown;
    int     scheduler;
}   t_config;

int	ft_atoi(const char *str);
int	ft_strcmp(const char *s1, const char *s2);

#endif
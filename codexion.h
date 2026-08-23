#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <limits.h>
# include <stddef.h>
# include <stdio.h>
# include <pthread.h> //mutex: init destroy lock unlock
					  //threads: create join detach
# include <sys/time.h> //gettimeofday

typedef struct s_coder
{
	int	coder_id;
	int	nb_compiles;
	t_dongle	left_dongle;
	t_dongle	right_dongle;
	pthread_t	thread_id;
}   t_coder;

typedef struct s_dongle
{
	pthread_mutex_t	dongle;
	int	dongle_id;
}	t_dongle;

typedef struct s_config
{
	long    nb_coders;
	long    time_to_burnout;
	long    time_to_compile;
	long    time_to_debug;
	long    time_to_refactor;
	int     nb_compiles_required;
	long    dongle_cooldown;
	int     scheduler;
	long	start;
	int		end; //evryone has reached the max number of compile or burnout (0/1)
}   t_config;

int	ft_atoi(const char *str);
int	ft_strcmp(const char *s1, const char *s2);

#endif
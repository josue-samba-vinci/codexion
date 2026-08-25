#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <limits.h>
# include <stddef.h>
# include <stdio.h>
# include <unistd.h>  //sleep write
# include <pthread.h> //mutex: init destroy lock unlock  threads: create join detach
# include <sys/time.h> //gettimeofday

typedef struct s_config t_config;

typedef struct s_request
{
	int		coder_id;
	long	key;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
    pthread_mutex_t  mutex;
    pthread_cond_t   cond;
    int              id;
    int              available;
    long             free_at;
    t_heap           waiters;
}   t_dongle;


typedef struct s_coder
{
	int	id;
	int	compile_count;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	pthread_t	thread_id;
	t_config	*table;
}   t_coder;

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
	int		end;
	t_coder	*coders;
	t_dongle	*dongles;
}   t_config;

int	ft_atoi(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
int	parser(int argc, char **argv, t_config *tab);

#endif
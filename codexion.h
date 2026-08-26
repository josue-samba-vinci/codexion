/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <limits.h>
# include <stddef.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define FIFO 0
# define EDF 1

typedef struct s_config	t_config;

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
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				id;
	int				available;
	long			free_at;
	t_heap			waiters;
	long			ticket;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile;
	pthread_t		thread_id;
	t_dongle		*first_dongle;
	t_dongle		*second_dongle;
	t_config		*config;
	pthread_mutex_t	mutex;
}	t_coder;

typedef struct s_config
{
	int				nb_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				nb_compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	long			start;
	int				end;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	state_lock;
	pthread_t		monitor;

}	t_config;

int		ft_atoi(const char *str);
int		ft_strcmp(const char *s1, const char *s2);
int		parser(int argc, char **argv, t_config *config);
int		init_dongles(t_config *config);
void	assign_dongle(t_coder *coder,
			t_dongle *dongles, int coder_pos, int nb_coders);
int		init_coders(t_config *config);
int		init_config(t_config *config);
int		only_numbers(char **argv);
int		number_of_coder(char **argv);
long	get_time_ms(void);
int		sim_is_over(t_config *config);
void	log_action(t_coder *coder, char *msg);
void	precise_sleep(long duration_ms, t_config *config);
void	take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
int		start_simulation(t_config *config);
void	handle_one_coder(t_coder *coder);
void	*coder_routine(void *arg);
void	update_compile(t_coder *coder);

#endif
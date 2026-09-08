/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                           :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wake_all_dongles(t_config *config)
{
	int			i;
	t_dongle	*dongle;

	i = 0;
	while (i < config->nb_coders)
	{
		dongle = config->dongles + i;
		pthread_mutex_lock(&dongle->mutex);
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->mutex);
		i++;
	}
}

int	check_burnout(t_config *config)
{
	int		i;
	long	coder_last_compile;
	t_coder	*coder;

	i = 0;
	while (i < config->nb_coders)
	{
		coder = config->coders + i;
		pthread_mutex_lock(&coder->mutex);
		coder_last_compile = coder->last_compile;
		pthread_mutex_unlock(&coder->mutex);
		if (get_time_ms() - coder_last_compile > config->time_to_burnout)
			return (coder->id);
		i++;
	}
	return (-1);
}

int	finished(t_config *config)
{
	int		i;
	int		compile_count;
	t_coder	*coder;

	i = 0;
	if (config->nb_compiles_required == 0)
		return (1);
	while (i < config->nb_coders)
	{
		coder = config->coders + i;
		pthread_mutex_lock(&coder->mutex);
		compile_count = coder->compile_count;
		pthread_mutex_unlock(&coder->mutex);
		if (compile_count < config->nb_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	stop_simulation(t_config *config, int coder_id)
{
	pthread_mutex_lock(&config->state_lock);
	config->end = 1;
	pthread_mutex_unlock(&config->state_lock);
	wake_all_dongles(config);
	if (coder_id >= 0)
	{
		pthread_mutex_lock(&config->print_lock);
		printf("%ld %d burned out\n", get_time_ms() - config->start, coder_id);
		pthread_mutex_unlock(&config->print_lock);
	}
}

void	*monitor_routine(void *arg)
{
	t_config	*config;
	int			burned_out;

	config = (t_config *)arg;
	while (1)
	{
		burned_out = check_burnout(config);
		if (burned_out >= 0)
		{
			stop_simulation(config, burned_out);
			return (NULL);
		}
		if (finished(config))
		{
			stop_simulation(config, -1);
			return (NULL);
		}
		usleep(1000);
	}
}

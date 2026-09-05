/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	update_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile = get_time_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
}

void	handle_one_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->first_dongle->mutex);
	log_action(coder, "has taken a dongle");
	precise_sleep(coder->config->time_to_burnout, coder->config);
	pthread_mutex_unlock(&coder->first_dongle->mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->config->nb_coders == 1)
	{
		handle_one_coder(coder);
		return (NULL);
	}
	while (!sim_is_over(coder->config))
	{
		take_dongles(coder);
		update_compile(coder);
		log_action(coder, "is compiling");
		precise_sleep(coder->config->time_to_compile, coder->config);
		release_dongles(coder);
		log_action(coder, "is debugging");
		precise_sleep(coder->config->time_to_debug, coder->config);
		log_action(coder, "is refactoring");
		precise_sleep(coder->config->time_to_refactor, coder->config);
	}
	return (NULL);
}

int	start_simulation(t_config *config)
{
	int		i;
	t_coder	*coder;

	i = 0;
	config->start = get_time_ms();
	while (i < config->nb_coders)
	{
		coder = config->coders + i;
		coder->last_compile = config->start;
		pthread_create(&coder->thread_id, NULL, coder_routine, coder);
		i++;
	}
	pthread_create(&config->monitor, NULL, monitor_routine, config);
	pthread_join(config->monitor, NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		pthread_join(config->coders[i].thread_id, NULL);
		i++;
	}
	return (1);
}

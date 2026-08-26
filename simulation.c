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

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	return (NULL);
}

int start_simulation(t_config *config)
{
	int	i;
	t_coder *coder;

	i = 0;
	config->start = get_time_ms();
	while (i < config->nb_coders)
	{
		coder = config->coders + i;
		coder->last_compile = config->start;
		pthread_create(&coder->thread_id, NULL, coder_routine, coder);
		i++;
	}
	i = 0;
	while (i < config->nb_coders)
	{
		pthread_join(config->coders[i].thread_id, NULL);
		i++;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles(t_config *config)
{
	t_dongle	*dongle;
	int			i;

	config->dongles = malloc(sizeof(t_dongle) * config->nb_coders);
	if (!config->dongles)
		return (0);
	i = 0;
	while (i < config->nb_coders)
	{
		dongle = config->dongles + i;
		pthread_mutex_init(&dongle->mutex, NULL);
		pthread_cond_init(&dongle->cond, NULL);
		dongle->id = i;
		dongle->available = 1;
		dongle->free_at = 0;
		dongle->waiters.capacity = 0;
		dongle->waiters.size = 0;
		dongle->waiters.data = NULL;
		dongle->ticket = 0;
		i++;
	}
	return (1);
}

void	assign_dongle(
	t_coder *coder, t_dongle *dongles, int coder_pos, int nb_coders
)
{
	int	id_first_dongle;
	int	id_second_dongle;

	id_first_dongle = dongles[(coder_pos) % nb_coders].id;
	id_second_dongle = dongles[(coder_pos + 1) % nb_coders].id;
	if (id_first_dongle > id_second_dongle)
	{
		coder->second_dongle = &dongles[(coder_pos) % nb_coders];
		coder->first_dongle = &dongles[(coder_pos + 1) % nb_coders];
		return ;
	}
	coder->first_dongle = &dongles[(coder_pos) % nb_coders];
	coder->second_dongle = &dongles[(coder_pos + 1) % nb_coders];
}

int	init_coders(t_config *config)
{
	int		i;
	t_coder	*coder;

	config->coders = malloc(sizeof(t_coder) * config->nb_coders);
	if (!config->coders)
		return (0);
	i = 0;
	while (i < config->nb_coders)
	{
		coder = config->coders + i;
		pthread_mutex_init(&coder->mutex, NULL);
		assign_dongle(coder, config->dongles, i, config->nb_coders);
		coder->id = i + 1;
		coder->compile_count = 0;
		coder->config = config;
		coder->last_compile = 0;
		i++;
	}
	return (1);
}

int	init_config(t_config *config)
{
	pthread_mutex_init(&config->print_lock, NULL);
	pthread_mutex_init(&config->state_lock, NULL);
	config->end = 0;
	if (!init_dongles(config))
		return (0);
	if (!init_coders(config))
		return (0);
	return (1);
}

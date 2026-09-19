/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_dongles(t_dongle *dongle, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&dongle[i].mutex);
		pthread_cond_destroy(&dongle[i].cond);
		free(dongle[i].waiters.data);
		i++;
	}
}

void	destroy_coders(t_coder *coder, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&coder[i].mutex);
		i++;
	}
}

void	clean_config(t_config *config)
{
	if (config->dongles)
	{
		destroy_dongles(config->dongles, config->nb_coders);
		free(config->dongles);
		config->dongles = NULL;
	}
	if (config->coders)
	{
		destroy_coders(config->coders, config->nb_coders);
		free(config->coders);
		config->coders = NULL;
	}
	pthread_mutex_destroy(&config->print_lock);
	pthread_mutex_destroy(&config->state_lock);
}

int	fail_dongles(t_config *config, int n)
{
	destroy_dongles(config->dongles, n);
	free(config->dongles);
	config->dongles = NULL;
	return (0);
}

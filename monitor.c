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

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

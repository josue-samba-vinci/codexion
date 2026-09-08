/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	not_available_path(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;

	while (!available_to_take(coder, dongle))
	{
		if (sim_is_over(coder->config))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		if (dongle->available == 1 && get_time_ms() < dongle->free_at)
		{
			long_to_timespec(dongle->free_at, &ts);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	return (1);
}

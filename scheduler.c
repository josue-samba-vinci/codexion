/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	choose_type_ticket(t_coder *coder, t_dongle *dongle)
{
	if (coder->config->scheduler == 0)
		return (fifo_key(dongle));
	else
		return (edf_key(coder));
}

long	fifo_key(t_dongle *dongle)
{
	return (dongle->ticket++);
}

long	edf_key(t_coder *coder)
{
	long	nearest_burnout;

	pthread_mutex_lock(&coder->mutex);
	nearest_burnout = (coder->last_compile
			+ coder->config->time_to_burnout);
	pthread_mutex_unlock(&coder->mutex);
	return (nearest_burnout);
}

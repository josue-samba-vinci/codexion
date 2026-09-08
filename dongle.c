/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_one_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request		request;

	pthread_mutex_lock(&dongle->mutex);
	request.coder_id = coder->id;
	request.key = choose_type_ticket(coder, dongle);
	heap_push(&dongle->waiters, request);
	if (not_available_path(coder, dongle))
	{
		heap_pop(&dongle->waiters);
		dongle->available = 0;
		pthread_mutex_unlock(&dongle->mutex);
		log_action(coder, "has taken a dongle");
		return (1);
	}
	return (0);
}

int	take_dongles(t_coder *coder)
{
	if (!take_one_dongle(coder, coder->first_dongle))
		return (0);
	if (!take_one_dongle(coder, coder->second_dongle))
	{
		release_one_dongle(coder, coder->first_dongle);
		return (0);
	}
	return (1);
}

int	available_to_take(t_coder *coder, t_dongle *dongle)
{
	if (dongle->available == 1
		&& dongle->free_at <= get_time_ms()
		&& heap_top(&dongle->waiters)->coder_id == coder->id)
		return (1);
	return (0);
}

void	release_one_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle-> mutex);
	dongle->available = 1;
	dongle->free_at = get_time_ms() + coder->config->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle-> mutex);
}

void	release_dongles(t_coder *coder)
{
	release_one_dongle(coder, coder->second_dongle);
	release_one_dongle(coder, coder->first_dongle);
}

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

void	take_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->first_dongle->mutex);
	log_action(coder, "has taken a dongle");
	pthread_mutex_lock(&coder->second_dongle->mutex);
	log_action(coder, "has taken a dongle");
}

void	release_dongles(t_coder *coder)
{
	release_one_dongle(coder, coder->second_dongle);
	release_one_dongle(coder, coder->first_dongle);
}

void release_one_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle-> mutex);
	dongle->available = 1; //nobody is having it because i just releasedf it
	dongle->free_at = get_time_ms() + coder->config->dongle_cooldown;// setting the time it will be available
	pthread_cond_broadcast(&dongle->cond);//wakes up everyone (signal would wake up only one person BAD)
	pthread_mutex_unlock(&dongle-> mutex);
}

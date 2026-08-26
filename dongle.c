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
	pthread_mutex_unlock(&coder->second_dongle->mutex);
	pthread_mutex_unlock(&coder->first_dongle->mutex);
}
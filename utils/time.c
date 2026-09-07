/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	precise_sleep(long duration_ms, t_config *config)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < duration_ms)
	{
		if (sim_is_over(config))
			return ;
		usleep(100);
	}
}

void	long_to_timespec(long ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = ms % 1000 * 1000000;
	return ;
}
int main()
{
	struct timespec ts;

	long ms = get_time_ms();
	long_to_timespec(ms, &ts);
	printf("tv_sec = %ld, tv_nsec = %ld", ts.tv_sec, ts.tv_nsec);
}
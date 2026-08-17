/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include ""

int	ft_atoi(const char *str)
{
	size_t	i;
	long	nb;

	i = 0;
	nb = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (nb > (nb * 10) + (str[i] - '0') && sign == 1)
			return ((int)LONG_MAX);
		if (nb > (nb * 10) + (str[i] - '0') && sign == -1)
			return ((int)LONG_MIN);
		nb = (nb * 10) + (str[i] - '0');
		i++;
	}
	return ((int)(nb * sign));
}
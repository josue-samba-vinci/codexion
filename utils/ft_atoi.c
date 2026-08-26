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

#include "codexion.h"

int	ft_atoi(const char *str)
{
	size_t	i;
	long	nb;

	if (str[0] == '\0')
		return (-1);
	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			nb = (nb * 10) + (str[i] - '0');
			if (nb > INT_MAX)
				return (-1);
			i++;
		}
		else
			return (-1);
	}
	return ((int)(nb));
}

/*#include <stdio.h>
int main()
{
    char *str1 = "12";
    char *str2 = "1234567890";
    char *str3 = "123maman";
	char *str4 = "2147483647";

    printf("%d\n", ft_atoi(str1));
    printf("%d\n", ft_atoi(str2));
    printf("%d\n", ft_atoi(str3));
    printf("%d", ft_atoi(str4));
}*/
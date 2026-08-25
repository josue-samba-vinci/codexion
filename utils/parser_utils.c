#include "codexion.h"

int number_of_coder(char **argv)
{
	if (ft_atoi(argv[1]) == 0)
		return (0);
	return (1);
}

int only_numbers(char **argv)
{
	int	i;
	int j;
	
	i = 1;
	while (i <= 7)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
			{
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
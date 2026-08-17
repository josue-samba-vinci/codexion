#include "codexion.h"

int only_numbers(int argc, char **argv)
{
	int	i;
	int j;
	
	i = 1;
	j = 0;
	while (argv[i])
	{
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

int number_of_coder(char **argv)
{
	if (ft_strcmp(argv[1], "0") == 0)
	{
		printf("%s", "Please enter at least 1 coder");
		return (0);
	}
	return (1);
}

int validate_data(char **argv)
{
	int	i;

	i = 0;
	while (i <= 7)
	{
		if (ft_atoi(argv[i]) == -1)
			return (0);
		i++;
	}
	if ((ft_strcmp(argv[i], "edf") == 0 || (ft_strcmp(argv[i], "fifo") == 0)))
		return (1);
	return (0);
}

t_config input_data(char **argv)
{
	t_config tab;
	tab.nb_coders = ft_atoi(argv[1]);
	tab.time_to_burnout = ft_atoi(argv[2]);
	tab.time_to_compile = ft_atoi(argv[3]);
	tab.time_to_debug = ft_atoi(argv[4]);
	tab.time_to_refactor = ft_atoi(argv[5]);
	tab.nb_compiles_required = ft_atoi(argv[6]);
	tab.dongle_cooldown = ft_atoi(argv[7]);
	if (ft_strcmp(argv[8],"fifo") == 0)
		tab.scheduler = 0;
	else
		tab.scheduler = 1;
	return (tab);
}

t_config validator(int argc, char **argv)
{
	if (argc != 9)
		return (0);
	if (!only_numbers(argc, argv))
	{
		printf("%s\n", "please put only numbers except for the last argument");
		printf("%s", "./a.out <number1> <number2> <number3> <number4> ");
		printf("%s", "<number5> <number6> <number7> <fifo/edf>");
		return (0);
	}
	if (!number_of_coder(argv))
		return (0);
	if (!validate_data(argv))
		return (0);
	return input_data(argv);
}
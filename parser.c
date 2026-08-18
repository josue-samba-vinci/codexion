#include "codexion.h"

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

int number_of_coder(char **argv)
{
	if (ft_atoi(argv[1]) == 0)
		return (0);
	return (1);
}



int validate_data(char **argv)
{
	int	i;

	i = 1;
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

void input_data(char **argv, t_config *tab)
{
	tab->nb_coders = ft_atoi(argv[1]);
	tab->time_to_burnout = ft_atoi(argv[2]);
	tab->time_to_compile = ft_atoi(argv[3]);
	tab->time_to_debug = ft_atoi(argv[4]);
	tab->time_to_refactor = ft_atoi(argv[5]);
	tab->nb_compiles_required = ft_atoi(argv[6]);
	tab->dongle_cooldown = ft_atoi(argv[7]);
	if (ft_strcmp(argv[8],"fifo") == 0)
		tab->scheduler = 0;
	else
		tab->scheduler = 1;
	return ;
}

int validator(int argc, char **argv)
{
	if (argc != 9)
		return (0);
	if (!only_numbers(argv))
		return (0);
	if (!number_of_coder(argv))
		return (0);
	if (!validate_data(argv))
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_config tab;
	if (validator(argc, argv))
	{
		input_data(argv, &tab);
		printf("%s%d\n", "number of coders : ", tab.nb_coders);
		printf("%s%ld\n", "time to burnout : ", tab.time_to_burnout);
		printf("%s%ld\n", "time to compile : ", tab.time_to_compile);
		printf("%s%ld\n", "time to debug : ", tab.time_to_debug);
		printf("%s%ld\n", "time to refactor : ", tab.time_to_refactor);
		printf("%s%d\n", "number of compilation required : ", tab.nb_compiles_required);
		printf("%s%ld\n", "dongle cooldown time : ", tab.dongle_cooldown);
		printf("%s%d\n", "scheduler type : ", tab.scheduler);
		return (0);
	}
	printf("%s\n", "Please put only numbers except for the last argument");
	printf("%s", "./a.out <number1_greater_than_0> <positive_number2> <positive_number3> <positive_number4> ");
	printf("%s", "<positive_number5> <positive_number6> <positive_number7> <fifo/edf>");
	return (1);
}
#include "codexion.h"

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

void input_data(char **argv, t_config *config)
{
	config->nb_coders = ft_atoi(argv[1]);
	config->time_to_burnout = ft_atoi(argv[2]);
	config->time_to_compile = ft_atoi(argv[3]);
	config->time_to_debug = ft_atoi(argv[4]);
	config->time_to_refactor = ft_atoi(argv[5]);
	config->nb_compiles_required = ft_atoi(argv[6]);
	config->dongle_cooldown = ft_atoi(argv[7]);
	if (ft_strcmp(argv[8],"fifo") == 0)
		config->scheduler = 0;
	else
		config->scheduler = 1;
	config->end = 0;
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

int	parser(int argc, char **argv, t_config *config)
{
	if (validator(argc, argv))
	{
		input_data(argv, config);
		return (1);
	}
	printf("%s\n", "Please put only numbers except for the last argument");
	printf("%s", "./codexion <number1_greater_than_0> <positive_number2> <positive_number3> <positive_number4> ");
	printf("%s", "<positive_number5> <positive_number6> <positive_number7> <fifo/edf>");
	return (0);
}
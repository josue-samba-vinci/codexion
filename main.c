#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config	config;

	if (!parser(argc, argv, &config))
		return (1);
	printf("number of coders : %d\n", config.nb_coders);
	printf("time to burnout : %ld\n", config.time_to_burnout);
	printf("time to compile : %ld\n", config.time_to_compile);
	printf("time to debug : %ld\n", config.time_to_debug);
	printf("time to refactor : %ld\n", config.time_to_refactor);
	printf("number of compilation required : %d\n",
		config.nb_compiles_required);
	printf("dongle cooldown time : %ld\n", config.dongle_cooldown);
	printf("scheduler type : %d\n", config.scheduler);
	if (!init_config(&config))
		printf("The malloc has failed");
}

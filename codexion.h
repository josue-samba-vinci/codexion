typedef struct s_config
{
    int     nb_coders;
    long    time_to_burnout;
    long    time_to_compile;
    long    time_to_debug;
    long    time_to_refactor;
    int     nb_compiles_required;
    long    dongle_cooldown;
    int     scheduler;
}   t_config;
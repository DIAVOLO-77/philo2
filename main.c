#include "philo.h"
#include <string.h>

int	run_philosophers(t_data *data, int argc, char **argv)
{
	if (init_data(data, argc, argv))
		return (1);
	if (init_mutexes(data))
	{
		free_all(data);
		return (1);
	}
	if (init_philosophers(data))
	{
		free_all(data);
		return (1);
	}
	if (start_simulation(data))
	{
		free_all(data);
		return (1);
	}
	free_all(data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (check_args(argc, argv))
	{
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat "
			"time_to_sleep [number_of_times_each_philosopher_must_eat]\n",
			argv[0]);
		return (1);
	}
	return (run_philosophers(&data, argc, argv));
}

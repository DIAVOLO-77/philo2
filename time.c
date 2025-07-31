#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return ((tv.tv_sec * (long long)1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(long long time_ms)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time_ms)
		usleep(500);
}

int	start_simulation(t_data *data)
{
	data->start_time = get_time();
	if (data->start_time == -1)
		return (1);
	if (data->nb_philo == 1)
	{
		display_message(&data->philosophers[0], "has taken a fork");
		precise_sleep(data->time_to_die);
		printf("%lld 1 died\n", (long long)data->time_to_die);
		return (0);
	}
	return (create_threads(data));
}

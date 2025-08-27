#include "philo.h"

static void	handle_one_philo(t_data *data)
{
	precise_sleep(data->time_to_die);
	printf("%d 1 died\n", data->time_to_die);
	pthread_mutex_lock(&data->data_mutex);
	data->stop = true;
	pthread_mutex_unlock(&data->data_mutex);
}

static int	check_philo_death(t_data *data, int i)
{
	long long	last_meal;
	bool		stopped;
	long long	death_ts;

	pthread_mutex_lock(&data->data_mutex);
	last_meal = data->philosophers[i].last_meal;
	stopped = data->stop;
	pthread_mutex_unlock(&data->data_mutex);
	if ((get_time() - last_meal) > data->time_to_die && !stopped)
	{
		death_ts = (last_meal + data->time_to_die) - data->start_time;
		pthread_mutex_lock(&data->write_mutex);
		printf("%lld %d died\n",
			death_ts,
			data->philosophers[i].id);
		pthread_mutex_unlock(&data->write_mutex);
		pthread_mutex_lock(&data->data_mutex);
		data->stop = true;
		pthread_mutex_unlock(&data->data_mutex);
		return (1);
	}
	return (0);
}

static int	check_all_ate(t_data *data)
{
	int	i;
	int	finished;

	if (data->must_eat == -1)
		return (0);
	i = -1;
	finished = 0;
	while (++i < data->nb_philo)
	{
		pthread_mutex_lock(&data->data_mutex);
		if (data->philosophers[i].nb_meals >= data->must_eat)
			finished++;
		pthread_mutex_unlock(&data->data_mutex);
	}
	if (finished == data->nb_philo)
	{
		pthread_mutex_lock(&data->data_mutex);
		data->stop = true;
		pthread_mutex_unlock(&data->data_mutex);
		return (1);
	}
	return (0);
}

static void	monitor_philosophers(t_data *data)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < data->nb_philo)
		{
			if (check_philo_death(data, i))
				return ;
		}
		if (check_all_ate(data))
			return ;
		usleep(500);
	}
}

void	*monitoring_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->nb_philo == 1)
	{
		handle_one_philo(data);
		return (NULL);
	}
	monitor_philosophers(data);
	return (NULL);
}

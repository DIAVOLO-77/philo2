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
	long long	time;

	pthread_mutex_lock(&data->data_mutex);
	time = get_time() - data->philosophers[i].last_meal;
	if (time > data->time_to_die && !data->stop)
	{
		printf("%lld %d died\n",
			get_time() - data->start_time,
			data->philosophers[i].id);
		data->stop = true;
		pthread_mutex_unlock(&data->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->data_mutex);
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

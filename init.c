#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->stop = false;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	return (0);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->write_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->data_mutex, NULL))
		return (1);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->philosophers = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philosophers)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].nb_meals = 0;
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->nb_philo;
		data->philosophers[i].last_meal = 0;
		data->philosophers[i].data = data;
		i++;
	}
	return (0);
}

int	create_threads(t_data *data)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philosophers[i].last_meal = data->start_time;
		if (pthread_create(&data->philosophers[i].thread_id, NULL,
				philosopher_routine, &data->philosophers[i]))
			return (1);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitoring_routine, data))
		return (1);
	return (join_threads(data, monitor));
}

int	join_threads(t_data *data, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_join(data->philosophers[i].thread_id, NULL))
			return (1);
		i++;
	}
	if (pthread_join(monitor, NULL))
		return (1);
	return (0);
}

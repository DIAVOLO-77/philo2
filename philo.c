#include "philo.h"

void	eat(t_philo *philo)
{
	long long	start;

	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	display_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	display_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->data_mutex);
	display_message(philo, "is eating");
	{
		start = get_time();
		while (!is_simulation_over(philo)
			&& (get_time() - start) < philo->data->time_to_eat)
			usleep(500);
	}
	if (!is_simulation_over(philo))
	{
		pthread_mutex_lock(&philo->data->data_mutex);
		philo->nb_meals++;
		pthread_mutex_unlock(&philo->data->data_mutex);
	}
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
}

void	sleep_think(t_philo *philo)
{
	long long	start;

	display_message(philo, "is sleeping");
	{
		start = get_time();
		while (!is_simulation_over(philo)
			&& (get_time() - start) < philo->data->time_to_sleep)
			usleep(500);
	}
	if (!is_simulation_over(philo))
		display_message(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->data_mutex);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_over(philo))
	{
		eat(philo);
		pthread_mutex_lock(&philo->data->data_mutex);
		if (philo->data->must_eat != -1
			&& philo->nb_meals >= philo->data->must_eat)
		{
			pthread_mutex_unlock(&philo->data->data_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->data_mutex);
		sleep_think(philo);
	}
	return (NULL);
}

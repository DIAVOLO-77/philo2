#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	display_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	display_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->data_mutex);
	display_message(philo, "is eating");
	precise_sleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->nb_meals++;
	pthread_mutex_unlock(&philo->data->data_mutex);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
}

void	sleep_think(t_philo *philo)
{
	display_message(philo, "is sleeping");
	precise_sleep(philo->data->time_to_sleep);
	display_message(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
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

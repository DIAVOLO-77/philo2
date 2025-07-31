#include "philo.h"

int	check_stop_conditions(t_data *data)
{
	int			i;
	long long	current_time;
	long long	time_since_last_meal;
	int			finished;

	i = -1;
	finished = 0;
	current_time = get_time();
	while (++i < data->nb_philo)
	{
		pthread_mutex_lock(&data->data_mutex);
		time_since_last_meal = current_time - data->philosophers[i].last_meal;
		if (time_since_last_meal > data->time_to_die)
		{
			display_message(&data->philosophers[i], "died");
			data->stop = true;
			pthread_mutex_unlock(&data->data_mutex);
			return (1);
		}
		if (data->must_eat != -1
			&& data->philosophers[i].nb_meals >= data->must_eat)
			finished++;
		pthread_mutex_unlock(&data->data_mutex);
	}
	return (data->must_eat != -1 && finished == data->nb_philo);
}

void	release_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	}
}

bool	is_simulation_over(t_philo *philo)
{
	bool	result;

	pthread_mutex_lock(&philo->data->data_mutex);
	result = philo->data->stop;
	pthread_mutex_unlock(&philo->data->data_mutex);
	return (result);
}

void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->data_mutex);
}

void	update_meal_count(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->nb_meals++;
	pthread_mutex_unlock(&philo->data->data_mutex);
}

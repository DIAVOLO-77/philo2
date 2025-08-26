#include "philo.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;
	int	digit;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (result > (INT_MAX - digit) / 10)
			return (-1);
		result = result * 10 + digit;
		i++;
	}
	return (result * sign);
}

void	display_message(t_philo *philo, char *message)
{
	long long	ts;
	bool		stopped;

	pthread_mutex_lock(&philo->data->write_mutex);
	pthread_mutex_lock(&philo->data->data_mutex);
	stopped = philo->data->stop;
	ts = get_time() - philo->data->start_time;
	if (!stopped)
		printf("%lld %d %s\n", ts, philo->id, message);
	pthread_mutex_unlock(&philo->data->data_mutex);
	pthread_mutex_unlock(&philo->data->write_mutex);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (1);
	i = 1;
	while (i < argc)
	{
		j = 0;
		if (argv[i][j] == '+')
			j++;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (1);
		i++;
	}
	return (0);
}

bool	is_simulation_over(t_philo *philo)
{
	bool	result;

	pthread_mutex_lock(&philo->data->data_mutex);
	result = philo->data->stop;
	pthread_mutex_unlock(&philo->data->data_mutex);
	return (result);
}

void	free_all(t_data *data)
{
	int	i;

	if (data->philosophers)
		free(data->philosophers);
	if (data->forks)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->data_mutex);
}

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>

#define INT_MAX 2147483647
#define INT_MIN -2147483648

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	bool			stop;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	data_mutex;
	t_philo			*philosophers;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				nb_meals;
	int				left_fork;
	int				right_fork;
	long long		last_meal;
	pthread_t		thread_id;
	t_data			*data;
}	t_philo;

void		*philosopher_routine(void *arg);
void		*monitoring_routine(void *arg);
void		eat(t_philo *philo);
void		sleep_think(t_philo *philo);

int			ft_atoi(const char *str);
void		display_message(t_philo *philo, char *message);
int			check_args(int argc, char **argv);
void		free_all(t_data *data);
bool		is_simulation_over(t_philo *philo);

long long	get_time(void);
void		precise_sleep(long long time_ms);
int			start_simulation(t_data *data);

int			init_data(t_data *data, int argc, char **argv);
int			init_mutexes(t_data *data);
int			init_philosophers(t_data *data);
int			create_threads(t_data *data);
int			join_threads(t_data *data, pthread_t monitor);

int			main(int argc, char **argv);
int			run_philosophers(t_data *data, int argc, char **argv);

#endif
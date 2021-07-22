/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvanbesi <tvanbesi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 11:56:54 by tvanbesi          #+#    #+#             */
/*   Updated: 2021/07/22 15:45:36 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

# define EAT		"is eating"
# define SLEEP		"is sleeping"
# define THINK		"is thinking"
# define TAKE		"has taken a fork"
# define DIE		"has died"

typedef enum e_status
{
	DEAD,
	ALIVE
}					t_status;

typedef enum e_fork
{
	FIRST,
	SECOND
}					t_fork;

typedef struct s_philo_data
{
	int				n_philo;
	int				ttd;
	int				tte;
	int				tts;
	int				n_eat;
}					t_philo_data;

typedef struct s_philosopher
{
	int				id;
	int				status;
	long int		tse_sec;
	long int		tse_usec;
	int				n_eat;
	int				ttd;
	int				tte;
	int				tts;
	pthread_mutex_t	*ffork;
	pthread_mutex_t	*sfork;
	pthread_mutex_t	*wlock;
	pthread_mutex_t	*rlock;
}					t_philosopher;

int				ft_atoi(const char *str);
int				ft_isspace(int c);
int				ft_isdigit(int c);

void			assign_data(t_philosopher *philosopher,
					t_philo_data philo_data);
void			assign_fork(int i, t_philosopher *philosopher,
					pthread_mutex_t **fork, int n);

pthread_mutex_t	**init_mutex(int n);
t_philo_data	init_philo_data(int argc, char **argv);
t_philosopher	**init_philosopher(t_philo_data philo_data,
					pthread_mutex_t **fork,
					pthread_mutex_t **rlock,
					pthread_mutex_t *wlock);
void			init_time(t_philosopher **philosopher);

int				done_eating(t_philosopher **philosopher);
int				all_alive(t_philosopher **philosopher);

int				thread(int n, t_philosopher **philosopher);
void			print_action(t_philosopher *philosopher,
					char *action, struct timeval time);

int				philo_starved(t_philosopher *philosopher, struct timeval time);
void			philo_take(t_philosopher *philosopher, pthread_mutex_t *rlock,
					int nfork);
void			philo_eat(t_philosopher *philosopher, pthread_mutex_t *rlock);
void			philo_sleep(t_philosopher *philosopher, pthread_mutex_t *rlock);
void			philo_think(t_philosopher *philosopher, pthread_mutex_t *rlock);
void			philo_die(t_philosopher *philosopher);

#endif

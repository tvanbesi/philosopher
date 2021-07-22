/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvanbesi <tvanbesi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 11:49:35 by tvanbesi          #+#    #+#             */
/*   Updated: 2021/07/22 10:51:41 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t
	**init_fork(int n)
{
	pthread_mutex_t	**fork;
	int				i;

	fork = malloc(sizeof(*fork) * n);
	if (!fork)
		return (NULL);
	i = 0;
	while (i < n)
	{
		fork[i] = malloc(sizeof(**fork));
		if (!fork[i++])
			return (NULL);
	}
	i = 0;
	while (i < n)
		if (pthread_mutex_init(fork[i++], NULL) != 0)
			return (NULL);
	return (fork);
}

t_philo_data
	init_philo_data(int argc, char **argv)
{
	t_philo_data	philo_data;

	philo_data.n_philo = ft_atoi(argv[1]);
	philo_data.ttd = ft_atoi(argv[2]);
	philo_data.tte = ft_atoi(argv[3]);
	philo_data.tts = ft_atoi(argv[4]);
	philo_data.n_eat = -1;
	if (argc == 6)
		philo_data.n_eat = ft_atoi(argv[5]);
	return (philo_data);
}

t_philosopher
	**init_philosopher(t_philo_data philo_data,
	pthread_mutex_t **fork, pthread_mutex_t *wlock)
{
	t_philosopher	**philosopher;
	int				i;

	philosopher = malloc(sizeof(*philosopher) * (philo_data.n_philo + 1));
	if (!philosopher)
		return (NULL);
	philosopher[philo_data.n_philo] = NULL;
	i = 0;
	while (i < philo_data.n_philo)
	{
		philosopher[i] = malloc(sizeof(**philosopher));
		if (!philosopher[i])
			return (NULL);
		philosopher[i]->id = i;
		philosopher[i]->status = ALIVE;
		assign_data(philosopher[i], philo_data);
		assign_fork(i, philosopher[i], fork, philo_data.n_philo);
		philosopher[i]->wlock = wlock;
		i++;
	}
	return (philosopher);
}

void
	init_time(t_philosopher **philosopher)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	while (*philosopher)
	{
		(*philosopher)->tse_sec = time.tv_sec;
		(*philosopher)->tse_usec = time.tv_usec;
		philosopher++;
	}
}

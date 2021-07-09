/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */

/*                                                    +:+ +:+         +:+     */
/*   By: tvanbesi <tvanbesi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 11:36:44 by tvanbesi          #+#    #+#             */
/*   Updated: 2021/07/07 18:04:42 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int
	done_eating(t_philosopher **philosopher)
{
	while (*philosopher)
	{
		if ((*philosopher)->n_eat != 0)
			return (0);
		philosopher++;
	}
	return (1);
}

static void
	*monitor_thread(void *arg)
{
	t_philosopher	**philosopher;
	struct timeval	time;
	int				anydead;

	philosopher = (t_philosopher**)arg;
	anydead = 0;
	while (!anydead && !done_eating(philosopher))
	{
		while (*philosopher)
		{
			gettimeofday(&time, NULL);
			if (time.tv_sec - (*philosopher)->tse_sec >= 0)
			{
				if ((time.tv_usec / 1000) - ((*philosopher)->tse_usec / 1000) > (*philosopher)->ttd - ((time.tv_sec - (*philosopher)->tse_sec) * 1000))
				{
					(*philosopher)->status = DEAD;
					pthread_mutex_lock((*philosopher)->wlock);
					gettimeofday(&time, NULL);
					printf("%ld+%ld Philosopher %d had died\n", time.tv_sec % 100, time.tv_usec / 1000, (*philosopher)->id);
					pthread_mutex_unlock((*philosopher)->wlock);
				}
			}
			if ((*philosopher)->status == DEAD)
			{
				anydead = 1;
				break ;
			}
			philosopher++;
		}
		philosopher = (t_philosopher**)arg;
	}
	return (NULL);
}

static void
	*philo_thread(void *arg)
{
	t_philosopher	*philosopher;
	struct timeval	time;

	philosopher = (t_philosopher*)arg;
	while (philosopher->status == ALIVE)
	{
		pthread_mutex_lock(philosopher->rfork);
		pthread_mutex_lock(philosopher->lfork);
		pthread_mutex_lock(philosopher->wlock);
		gettimeofday(&time, NULL);
		philosopher->tse_sec = time.tv_sec;
		philosopher->tse_usec = time.tv_usec;
		if (philosopher->status == ALIVE)
			printf("%ld+%ld Philosopher %d is eating\n", time.tv_sec % 100, time.tv_usec / 1000, philosopher->id);
		pthread_mutex_unlock(philosopher->wlock);
		if (philosopher->n_eat > 0)
			philosopher->n_eat--;
		usleep(philosopher->tte * 1000);
		pthread_mutex_unlock(philosopher->rfork);
		pthread_mutex_unlock(philosopher->lfork);
		pthread_mutex_lock(philosopher->wlock);
		gettimeofday(&time, NULL);
		if (philosopher->status == ALIVE)
			printf("%ld+%ld Philosopher %d is sleeping\n", time.tv_sec % 100, time.tv_usec / 1000, philosopher->id);
		pthread_mutex_unlock(philosopher->wlock);
		usleep(philosopher->tts * 1000);
		pthread_mutex_lock(philosopher->wlock);
		gettimeofday(&time, NULL);
		if (philosopher->status == ALIVE)
			printf("%ld+%ld Philosopher %d is thinking\n", time.tv_sec % 100, time.tv_usec / 1000, philosopher->id);
		pthread_mutex_unlock(philosopher->wlock);
	}
	return (NULL);
}

int
	main(int argc, char **argv)
{
	t_philosopher		**philosopher;
	pthread_t			*tid;
	pthread_t			monitor;
	pthread_mutex_t		**fork;
	pthread_mutex_t		wlock;
	t_philo_data		philo_data;
	int					i;

	//arg sanity check
	if (argc < 5 || argc > 6)
		return (-1);

	philo_data = init_philo_data(argc, argv);
	fork = init_fork(philo_data.n_philo);
	if (!fork)
		return (-1);
	if (pthread_mutex_init(&wlock, NULL) != 0)
		return (-1);
	philosopher = init_philosopher(philo_data, fork, &wlock);
	if (!philosopher)
		return (-1);

	if (!(tid = malloc(sizeof(*tid) * philo_data.n_philo)))
		return (-1);
	init_time(philosopher, &wlock);
	if (pthread_create(&monitor, NULL, &monitor_thread, philosopher) != 0)
		return (-1);
	i = 0;
	while (i < philo_data.n_philo)
	{
		if (pthread_create(&(tid[i]), NULL, &philo_thread, philosopher[i]) != 0)
			return (-1);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < philo_data.n_philo)
		pthread_mutex_destroy(fork[i++]);
	pthread_mutex_destroy(&wlock);
	//free all malloc and destroy all threads and mutexes
}

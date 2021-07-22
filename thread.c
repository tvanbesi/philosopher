/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 20:25:02 by user42            #+#    #+#             */
/*   Updated: 2021/07/22 16:19:03 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void
	free_philo(t_philosopher **philosopher)
{
	t_philosopher	**tmp;

	lock_philo(philosopher);
	if (all_alive(philosopher))
		pthread_mutex_lock((*philosopher)->wlock);
	tmp = philosopher;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(philosopher);
}

static void
	*monitor_routine(void *arg)
{
	t_philosopher	**philosopher;
	struct timeval	time;

	philosopher = (t_philosopher **)arg;
	while (!all_ready(philosopher))
		;
	while (all_alive(philosopher) && !done_eating(philosopher))
	{
		while (*philosopher)
		{
			gettimeofday(&time, NULL);
			if (time.tv_sec - (*philosopher)->tse_sec >= 0)
			{
				if (philo_starved(*philosopher, time))
				{
					philo_die(*philosopher);
					break ;
				}
			}
			philosopher++;
		}
		philosopher = (t_philosopher **)arg;
	}
	free_philo(philosopher);
	return (NULL);
}

static void
	*philo_routine(void *arg)
{
	t_philosopher	*philosopher;
	pthread_mutex_t	*rlock;
	pthread_mutex_t	*wlock;

	philosopher = (t_philosopher *)arg;
	rlock = philosopher->rlock;
	wlock = philosopher->wlock;
	philosopher->ready = 1;
	pthread_mutex_lock(rlock);
	if (philosopher->id % 2)
		usleep(philosopher->tte / 2 * 1000);
	pthread_mutex_unlock(rlock);
	while (philo_can_continue(philosopher, rlock))
	{
		philo_take(philosopher, rlock, wlock, FIRST);
		philo_take(philosopher, rlock, wlock, SECOND);
		philo_eat(philosopher, rlock, wlock);
		philo_sleep(philosopher, rlock, wlock);
		philo_think(philosopher, rlock, wlock);
	}
	return (NULL);
}

static int
	philo_thread(t_philosopher *philosopher)
{
	pthread_t	philo_t;

	if (pthread_create(&philo_t, NULL, &philo_routine, philosopher) != 0)
		return (-1);
	pthread_detach(philo_t);
	return (0);
}

int
	thread(int n, t_philosopher **philosopher)
{
	pthread_t	monitor_t;
	int			i;

	i = 0;
	while (i < n)
		if (philo_thread(philosopher[i++]) != 0)
			return (-1);
	if (pthread_create(&monitor_t, NULL, &monitor_routine, philosopher) != 0)
		return (-1);
	pthread_join(monitor_t, NULL);
	return (0);
}

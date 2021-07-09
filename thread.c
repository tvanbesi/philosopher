/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 20:25:02 by user42            #+#    #+#             */
/*   Updated: 2021/07/09 21:47:15 by user42           ###   ########.fr       */
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

static int
	all_alive(t_philosopher **philosopher)
{
	while (*philosopher)
	{
		if ((*philosopher)->status == DEAD)
			return (0);
		philosopher++;
	}
	return (1);
}

static void
	*monitor_routine(void *arg)
{
	t_philosopher	**philosopher;
	struct timeval	time;

	philosopher = (t_philosopher **)arg;
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
		philosopher = (t_philosopher**)arg;
	}
	return (NULL);
}

static void
	*philo_routine(void *arg)
{
	t_philosopher	*philosopher;
	struct timeval	time;

	philosopher = (t_philosopher *)arg;
	while (philosopher->status == ALIVE)
	{
		pthread_mutex_lock(philosopher->ffork);
		philo_take(philosopher);
		pthread_mutex_lock(philosopher->sfork);
		philo_take(philosopher);
		philo_eat(philosopher);
		philo_sleep(philosopher);
		philo_think(philosopher);
	}
	return (NULL);
}

int
	thread(int n, t_philosopher **philosopher)
{
	pthread_t	*philo_t;
	pthread_t	monitor_t;
	int			i;

	philo_t = malloc(sizeof(*philo_t) * n);
	if (!philo_t)
		return (-1);
	if (pthread_create(&monitor_t, NULL, &monitor_routine, philosopher) != 0)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (pthread_create(&(philo_t[i]), NULL,
				&philo_routine, philosopher[i]) != 0)
			return (-1);
		i++;
	}
	pthread_join(monitor_t, NULL);
	//free(philo_t);
	return (0);
}

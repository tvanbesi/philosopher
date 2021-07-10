/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 20:25:02 by user42            #+#    #+#             */
/*   Updated: 2021/07/10 14:29:52 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		philosopher = (t_philosopher **)arg;
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

static int
	philo_thread(t_philosopher *philosopher)
{
	pthread_t	philo_t;

	return (pthread_create(&philo_t, NULL, &philo_routine, philosopher));
}

int
	thread(int n, t_philosopher **philosopher)
{
	pthread_t	monitor_t;
	int			i;

	if (pthread_create(&monitor_t, NULL, &monitor_routine, philosopher) != 0)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (philo_thread(philosopher[i]) != 0)
			return (-1);
		i++;
	}
	pthread_join(monitor_t, NULL);
	return (0);
}

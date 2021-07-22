/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 21:01:49 by user42            #+#    #+#             */
/*   Updated: 2021/07/22 16:17:40 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void
	philo_take(t_philosopher *philosopher, pthread_mutex_t *rlock,
		pthread_mutex_t *wlock, int nfork)
{
	struct timeval	time;
	pthread_mutex_t	*fork;

	pthread_mutex_lock(rlock);
	if (philosopher->status == DEAD)
	{
		pthread_mutex_unlock(rlock);
		return ;
	}
	if (nfork == FIRST)
		fork = philosopher->ffork;
	else
		fork = philosopher->sfork;
	pthread_mutex_unlock(rlock);
	pthread_mutex_lock(fork);
	pthread_mutex_lock(wlock);
	gettimeofday(&time, NULL);
	print_action(philosopher, TAKE, time);
	pthread_mutex_unlock(wlock);
}

void
	philo_eat(t_philosopher *philosopher, pthread_mutex_t *rlock,
		pthread_mutex_t *wlock)
{
	struct timeval	time;

	pthread_mutex_lock(rlock);
	if (philosopher->status == DEAD)
	{
		pthread_mutex_unlock(rlock);
		return ;
	}
	pthread_mutex_unlock(rlock);
	pthread_mutex_lock(wlock);
	gettimeofday(&time, NULL);
	philosopher->tse_sec = time.tv_sec;
	philosopher->tse_usec = time.tv_usec;
	print_action(philosopher, EAT, time);
	pthread_mutex_unlock(wlock);
	pthread_mutex_lock(rlock);
	if (philosopher->n_eat > 0)
		philosopher->n_eat--;
	usleep(philosopher->tte * 1000);
	pthread_mutex_unlock(philosopher->ffork);
	pthread_mutex_unlock(philosopher->sfork);
	pthread_mutex_unlock(rlock);
}

void
	philo_sleep(t_philosopher *philosopher, pthread_mutex_t *rlock,
		pthread_mutex_t *wlock)
{
	struct timeval	time;

	pthread_mutex_lock(rlock);
	if (philosopher->status == DEAD)
	{
		pthread_mutex_unlock(rlock);
		return ;
	}
	pthread_mutex_unlock(rlock);
	pthread_mutex_lock(wlock);
	gettimeofday(&time, NULL);
	print_action(philosopher, SLEEP, time);
	pthread_mutex_unlock(wlock);
	pthread_mutex_lock(rlock);
	usleep(philosopher->tts * 1000);
	pthread_mutex_unlock(rlock);
}

void
	philo_think(t_philosopher *philosopher, pthread_mutex_t *rlock,
		pthread_mutex_t *wlock)
{
	struct timeval	time;

	pthread_mutex_lock(rlock);
	if (philosopher->status == DEAD)
	{
		pthread_mutex_unlock(rlock);
		return ;
	}
	pthread_mutex_unlock(rlock);
	pthread_mutex_lock(wlock);
	gettimeofday(&time, NULL);
	print_action(philosopher, THINK, time);
	pthread_mutex_unlock(wlock);
}

void
	philo_die(t_philosopher *philosopher)
{
	struct timeval	time;

	pthread_mutex_lock(philosopher->wlock);
	philosopher->status = DEAD;
	gettimeofday(&time, NULL);
	print_action(philosopher, DIE, time);
}

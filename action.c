/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 21:01:49 by user42            #+#    #+#             */
/*   Updated: 2021/07/22 13:20:15 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void
	philo_take(t_philosopher *philosopher)
{
	struct timeval	time;

	if (philosopher->status == DEAD)
		return ;
	pthread_mutex_lock(philosopher->wlock);
	gettimeofday(&time, NULL);
	print_action(philosopher, TAKE, time);
	pthread_mutex_unlock(philosopher->wlock);
}

void
	philo_eat(t_philosopher *philosopher)
{
	struct timeval	time;

	if (philosopher->status == DEAD)
		return ;
	pthread_mutex_lock(philosopher->wlock);
	gettimeofday(&time, NULL);
	philosopher->tse_sec = time.tv_sec;
	philosopher->tse_usec = time.tv_usec;
	print_action(philosopher, EAT, time);
	pthread_mutex_unlock(philosopher->wlock);
	if (philosopher->n_eat > 0)
		philosopher->n_eat--;
	usleep(philosopher->tte * 1000);
	pthread_mutex_unlock(philosopher->ffork);
	pthread_mutex_unlock(philosopher->sfork);
}

void
	philo_sleep(t_philosopher *philosopher)
{
	struct timeval	time;

	if (philosopher->status == DEAD)
		return ;
	pthread_mutex_lock(philosopher->wlock);
	gettimeofday(&time, NULL);
	print_action(philosopher, SLEEP, time);
	pthread_mutex_unlock(philosopher->wlock);
	usleep(philosopher->tts * 1000);
}

void
	philo_think(t_philosopher *philosopher)
{
	struct timeval	time;

	if (philosopher->status == DEAD)
		return ;
	pthread_mutex_lock(philosopher->wlock);
	gettimeofday(&time, NULL);
	print_action(philosopher, THINK, time);
	pthread_mutex_unlock(philosopher->wlock);
}

void
	philo_die(t_philosopher *philosopher)
{
	struct timeval	time;

	philosopher->status = DEAD;
	gettimeofday(&time, NULL);
	print_action(philosopher, DIE, time);
}

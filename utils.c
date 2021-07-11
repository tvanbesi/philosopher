/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 19:20:54 by user42            #+#    #+#             */
/*   Updated: 2021/07/11 11:14:57 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void
	assign_data(t_philosopher *philosopher, t_philo_data philo_data)
{
	philosopher->ttd = philo_data.ttd;
	philosopher->tte = philo_data.tte;
	philosopher->tts = philo_data.tts;
	philosopher->n_eat = philo_data.n_eat;
}

void
	assign_fork(int i, t_philosopher *philosopher,
	pthread_mutex_t **fork, int n)
{
	philosopher->ffork = fork[i];
	philosopher->sfork = fork[(i + 1) % n];
}

void
	print_action(t_philosopher *philosopher, char *action, struct timeval time)
{
	printf("%02ld%03ld Philosopher %d %s\n", time.tv_sec % 100,
		time.tv_usec / 1000, philosopher->id + 1, action);
}

int
	philo_starved(t_philosopher *philosopher, struct timeval time)
{
	if ((time.tv_usec / 1000) - (philosopher->tse_usec / 1000)
		> philosopher->ttd - ((time.tv_sec - philosopher->tse_sec) * 1000))
		return (1);
	return (0);
}

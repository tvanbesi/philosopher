/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvanbesi <tvanbesi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/22 16:13:44 by tvanbesi          #+#    #+#             */
/*   Updated: 2021/07/22 16:15:20 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void
	lock_philo(t_philosopher **philosopher)
{
	while (*philosopher)
	{
		pthread_mutex_lock((*philosopher)->rlock);
		philosopher++;
	}
}

int
	philo_can_continue(t_philosopher *philosopher, pthread_mutex_t *rlock)
{
	int	r;

	pthread_mutex_lock(rlock);
	r = (philosopher->status == ALIVE && philosopher->n_eat != 0);
	pthread_mutex_unlock(rlock);
	return (r);
}

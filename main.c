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

static void
	free_mutex(pthread_mutex_t **mutex, int n)
{
	while (n-- > 0)
		free(mutex[n]);
	free(mutex);
}

int
	main(int argc, char **argv)
{
	t_philosopher		**philosopher;
	pthread_mutex_t		**fork;
	pthread_mutex_t		**rlock;
	pthread_mutex_t		wlock;
	t_philo_data		philo_data;

	if (argc < 5 || argc > 6)
		return (-1);
	philo_data = init_philo_data(argc, argv);
	fork = init_mutex(philo_data.n_philo);
	rlock = init_mutex(philo_data.n_philo);
	if (!fork)
		return (-1);
	if (pthread_mutex_init(&wlock, NULL) != 0)
		return (-1);
	philosopher = init_philosopher(philo_data, fork, rlock, &wlock);
	if (!philosopher)
		return (-1);
	init_time(philosopher);
	if (thread(philo_data.n_philo, philosopher) != 0)
		return (-1);
	free_mutex(fork, philo_data.n_philo);
	free_mutex(rlock, philo_data.n_philo);
}

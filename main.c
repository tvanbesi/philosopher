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

int
	main(int argc, char **argv)
{
	t_philosopher		**philosopher;
	pthread_mutex_t		**fork;
	pthread_mutex_t		wlock;
	t_philo_data		philo_data;
	int					i;

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
	init_time(philosopher, &wlock);
	if (thread(philo_data.n_philo, philosopher) != 0)
		return (-1);
	i = 0;
	while (i < philo_data.n_philo)
		pthread_mutex_destroy(fork[i++]);
	pthread_mutex_destroy(&wlock);
	//free all malloc and destroy all threads and mutexes and stop all thread when one dude dies
}

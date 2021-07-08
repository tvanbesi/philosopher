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

static int
	init(t_philosopher **philosopher, int argc, char **argv, pthread_mutex_t **fork, pthread_mutex_t *wlock)
{
	int	n_philo;

	n_philo = ft_atoi(argv[1]);
	philosopher[n_philo] = NULL;
	while (n_philo-- > 0)
	{
		if (!(philosopher[n_philo] = malloc(sizeof(**philosopher))))
			return (-1);
		philosopher[n_philo]->id = n_philo;
		philosopher[n_philo]->status = ALIVE;
		if (argc == 6)
			philosopher[n_philo]->n_eat = ft_atoi(argv[5]);
		else
			philosopher[n_philo]->n_eat = -1;
		philosopher[n_philo]->ttd = ft_atoi(argv[2]);
		philosopher[n_philo]->tte = ft_atoi(argv[3]);
		philosopher[n_philo]->tts = ft_atoi(argv[4]);
		philosopher[n_philo]->rfork = fork[n_philo];
		if (n_philo > 0)
			philosopher[n_philo]->lfork = fork[n_philo - 1];
		else
			philosopher[n_philo]->lfork = fork[ft_atoi(argv[1]) - 1];
		philosopher[n_philo]->wlock = wlock;
	}
	return (0);
}

static void
	*monitor_thread(void *arg)
{
	t_philosopher	**philosopher;
	struct timeval	time;
	int				anydead;

	philosopher = (t_philosopher**)arg;
	pthread_mutex_lock(philosopher[0]->wlock);
	printf("Initializing\n");
	gettimeofday(&time, NULL);
	while (*philosopher)
	{
		(*philosopher)->tse_sec = time.tv_sec;
		(*philosopher)->tse_usec = time.tv_usec;
		philosopher++;
	}
	philosopher = (t_philosopher**)arg;
	pthread_mutex_unlock(philosopher[0]->wlock);
	anydead = 0;
	while (!anydead && !done_eating(philosopher))
	{
		while (*philosopher)
		{
			gettimeofday(&time, NULL);
			if (time.tv_sec - (*philosopher)->tse_sec > (*philosopher)->ttd / 1000)
			{
				if (time.tv_usec - (*philosopher)->tse_usec > (*philosopher)->ttd - ((time.tv_sec - (*philosopher)->tse_sec) * 1000))
				{
					(*philosopher)->status = DEAD;
					pthread_mutex_lock((*philosopher)->wlock);
					gettimeofday(&time, NULL);
					printf("%ld+%ld Philosopher %d had died\n", time.tv_sec % 100, time.tv_usec / 1000, (*philosopher)->id);
					pthread_mutex_unlock((*philosopher)->wlock);
				}
			}
			else if (time.tv_sec - (*philosopher)->tse_sec == (*philosopher)->ttd / 1000)
			{
				if (time.tv_usec - (*philosopher)->tse_usec > (*philosopher)->ttd)
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
	int					n_philo;
	int					i;

	if (argc < 5 || argc > 6)
		return (-1);
	n_philo = ft_atoi(argv[1]);
	if (!(philosopher = malloc(sizeof(*philosopher) * (n_philo + 1))))
		return (-1);
	if (!(fork = malloc(sizeof(*fork) * n_philo)))
		return (-1);
	i = 0;
	while (i < n_philo)
		if (!(fork[i++] = malloc(sizeof(**fork))))
			return (-1);
	if (pthread_mutex_init(&wlock, NULL) != 0)
		return (-1);
	i = 0;
	while (i < n_philo)
		if (pthread_mutex_init(fork[i++], NULL) != 0)
			return (-1);
	if (init(philosopher, argc, argv, fork, &wlock) != 0)
		return (-1);
	if (!(tid = malloc(sizeof(*tid) * n_philo)))
		return (-1);
	i = 0;
	if (pthread_create(&monitor, NULL, &monitor_thread, philosopher) != 0)
		return (-1);
	while (i < n_philo)
	{
		if (pthread_create(&(tid[i]), NULL, &philo_thread, philosopher[i]) != 0)
			return (-1);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < n_philo)
		pthread_mutex_destroy(fork[i++]);
	pthread_mutex_destroy(&wlock);
	//free all philosophers
}

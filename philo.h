/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvanbesi <tvanbesi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 11:56:54 by tvanbesi          #+#    #+#             */
/*   Updated: 2021/07/09 13:04:06 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef enum		e_status
{
	DEAD,
	ALIVE
}					t_status;

typedef struct		s_philo_data
{
	int				n_philo;
	int				ttd;
	int				tte;
	int				tts;
	int				n_eat;
}					t_philo_data;

typedef struct		s_philosopher
{
	int				id;
	int				status;
	long int		tse_sec;
	long int		tse_usec;
	int				n_eat;
	int				ttd;
	int				tte;
	int				tts;
	pthread_mutex_t	*rfork;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*wlock;
}					t_philosopher;

int		ft_atoi(const char *str);
int		ft_isspace(int c);
int		ft_isdigit(int c);

pthread_mutex_t	**init_fork(int n);
t_philo_data	init_philo_data(int argc, char **argv);
t_philosopher	**init_philosopher(t_philo_data philo_data, pthread_mutex_t **fork, pthread_mutex_t *wlock);
void			init_time(t_philosopher **philosopher, pthread_mutex_t *wlock);



#endif

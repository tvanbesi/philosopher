/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvanbesi <tvanbesi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 11:56:54 by tvanbesi          #+#    #+#             */
/*   Updated: 2021/07/07 19:49:26 by tvanbesi         ###   ########.fr       */
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

typedef struct		s_philosopher
{
	int				id;
	int				status;
	int				tse;
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

#endif

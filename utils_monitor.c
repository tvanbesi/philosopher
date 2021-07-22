/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 14:29:39 by user42            #+#    #+#             */
/*   Updated: 2021/07/22 16:04:52 by tvanbesi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int
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

int
	all_alive(t_philosopher **philosopher)
{
	while (*philosopher)
	{
		if ((*philosopher)->status == DEAD)
			return (0);
		philosopher++;
	}
	return (1);
}

int
	all_ready(t_philosopher **philosopher)
{
	while (*philosopher)
	{
		if (!(*philosopher)->ready)
			return (0);
		philosopher++;
	}
	return (1);
}

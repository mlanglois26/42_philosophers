/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:24:36 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/16 12:12:15 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_milli(int nb)
{
	nb *= 1000;
	return (nb);
}

// // start = start_time_to_do_smthg && curr = current_time
void	precise_usleep(long time_to_do_smthg)
{
	struct timeval	start;
	struct timeval	cur;
	long			tps_ecoule;
	long			reste;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&cur, NULL);
		tps_ecoule = (cur.tv_sec - start.tv_sec) * 1e6 + (cur.tv_usec
				- start.tv_usec);
		if (tps_ecoule >= time_to_do_smthg)
			break ;
		reste = time_to_do_smthg - tps_ecoule;
		if (reste > 1e4)
			usleep(1e4);
		else
			usleep(reste);
	}
}

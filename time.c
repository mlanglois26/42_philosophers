/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:24:36 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/16 09:30:37 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_milli(int nb)
{
	nb *= 1000;
	return (nb);
}

// start = start_time_to_do_smthg && curr = current_time
void	precise_usleep(long time_to_do_smthg)
{
	struct timeval	start;
	struct timeval	cur;
	long			tps_ecoule;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&cur, NULL);
		tps_ecoule = (cur.tv_sec - start.tv_sec) * 1e6 + (cur.tv_usec
				- start.tv_usec);
		if (tps_ecoule >= time_to_do_smthg)
			break ;
	}
}

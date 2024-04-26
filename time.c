/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:24:24 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/26 14:19:17 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_milli(int nb)
{
	nb *= 1000;
	return (nb);
}

// s = start_time_to_do_smthg && curr = current_time
void	precise_usleep(long time_to_do_smthg)
{
	struct timeval	s;
	struct timeval	cur;
	long			tps_ecoule;
	long			reste;

	gettimeofday(&s, NULL);
	while (1)
	{
		gettimeofday(&cur, NULL);
		tps_ecoule = (cur.tv_sec - s.tv_sec) * 1e6 + (cur.tv_usec - s.tv_usec);
		if (tps_ecoule >= time_to_do_smthg)
			break ;
		reste = time_to_do_smthg - tps_ecoule;
		if (reste > 1e4)
			usleep(reste / 2);
		else
			while (1)
			{
				gettimeofday(&cur, NULL);
				tps_ecoule = (cur.tv_sec - s.tv_sec) * 1e6 + (cur.tv_usec
						- s.tv_usec);
				if (tps_ecoule >= time_to_do_smthg)
					break ;
			}
	}
}

// void	precise_usleep(long time_to_do_smthg)
// {
// 	struct timeval	start_time;
// 	struct timeval	current_time;
// 	long			elapsed_time;
// 	long			remaining_time;

// 	gettimeofday(&start_time, NULL);
// 	while (1)
// 	{
// 		gettimeofday(&current_time, NULL);
// 		elapsed_time = (current_time.tv_sec - start_time.tv_sec) * 1e6
// 			+ (current_time.tv_usec - start_time.tv_usec);
// 		if (elapsed_time >= time_to_do_smthg)
// 			break ;
// 		remaining_time = time_to_do_smthg - elapsed_time;
// 		if (remaining_time > 1e4)
// 			usleep(remaining_time / 2);
// 		else
// 			usleep(remaining_time);
// 	}
// }

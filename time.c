/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:24:24 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/25 09:42:01 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_milli(int nb)
{
    nb *= 1000;
    return (nb);
}

void precise_usleep(long time_to_do_smthg)
{
    struct timeval start_time_to_do_smthg;
    struct timeval current_time;
    long tps_ecoule;
    long reste;

    gettimeofday(&start_time_to_do_smthg, NULL);
    while (1)
    {
        gettimeofday(&current_time, NULL);
        tps_ecoule = (current_time.tv_sec - start_time_to_do_smthg.tv_sec) * 1e6 + (current_time.tv_usec - start_time_to_do_smthg.tv_usec);
        if (tps_ecoule >= time_to_do_smthg)
            break;
        reste = time_to_do_smthg - tps_ecoule;
        if (reste > 1e4)
            usleep(reste / 2);
        else
            while (1)
            {
                gettimeofday(&current_time, NULL);
                tps_ecoule = (current_time.tv_sec - start_time_to_do_smthg.tv_sec) * 1e6 + (current_time.tv_usec - start_time_to_do_smthg.tv_usec);
                if (tps_ecoule >= time_to_do_smthg)
                    break;
            }
    }
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:32:10 by malanglo          #+#    #+#             */
/*   Updated: 2024/03/28 16:47:15 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int can_i_eat(t_program *program, t_philo *philo)
{
    int g;
    int d;
        printf("iiiiiiiiiiiiiiiiiiii\n");

    g = (philo->id + program->phil_count - 1) % program->phil_count;
    d = (philo->id + 1) % program->phil_count;

    if ((*program->philosophers[philo->id].last_meal_time) == 0)
    {
        (*program->philosophers[philo->id].last_meal_time) = (double)(program->philosophers[philo->id].life_starting_time);
    }
    if ((*program->philosophers[g].last_meal_time) == 0)
    {
        (*program->philosophers[g].last_meal_time) = (double)(program->philosophers[g].life_starting_time);
    }
    if ((*program->philosophers[d].last_meal_time) == 0)
    {
        (*program->philosophers[d].last_meal_time) = (double)(program->philosophers[d].life_starting_time);
    }
    
    if ((*(program->philosophers[g].state) == THINKING) && ((*program->philosophers[d].last_meal_time) > (*program->philosophers[philo->id].last_meal_time)))
    {
        // printf("philo de DROITE a plus faim!!!!!!!!!!!!!!!!!!!!\n");
        // return (0);
        return (0);
    }
    else if ((*(program->philosophers[d].state) == THINKING) && ((*program->philosophers[g].last_meal_time) > (*program->philosophers[philo->id].last_meal_time)))
    {
        // printf("philo de GAUCHE a plus faim!!!!!!!!!!!!!!!!!!!!\n");
        // return (0);
        return (0);
    }
    else
        return (1);
}

void pickup_forks(t_program *program, t_philo *philo)
{
    int left_fork_id;
    int right_fork_id;

    left_fork_id = philo->id;
    right_fork_id = (philo->id + 1) % program->phil_count;
    printf("ooooooooooooooooooo\n");
    if (can_i_eat(program, philo) == 1)
    {
        printf("aaaaaaaaaaaaaaaaaaaaaaaa\n");

        pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
        // printf("Philo %d has taken fork %d\n", philo->id, left_fork_id);

        pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
        // printf("Philo %d has taken fork %d\n", philo->id, right_fork_id);
    }
    else
        return ;
}

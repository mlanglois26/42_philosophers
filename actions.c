/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:48:09 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/25 13:10:26 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// // pas sure d avoir le droit de faire ca 
// int can_i_eat(t_program *program, t_philo *philo)
// {
//     int g;
//     int d;

//     g = (philo->id + program->phil_count - 1) % program->phil_count;
//     d = (philo->id + 1) % program->phil_count;

//     if (program->philosophers[g].last_meal_time > program->philosophers[philo->id].last_meal_time)
//         return (0);
//     else if (program->philosophers[d].last_meal_time > program->philosophers[philo->id].last_meal_time)
//         return (0);
//     else
//         return (1);
// }

// apres avoir pris les 2 forks
void update_meal_counter(t_philo *philo)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    
    pthread_mutex_lock(&philo->update_meal_counter_mutex);
    philo->meal_counter++;
    philo->last_meal_time = (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
    pthread_mutex_unlock(&philo->update_meal_counter_mutex);
}

void pickup_forks(t_program *program, t_philo *philo)
{

    int left_fork_id;
    int right_fork_id;

    left_fork_id = philo->id;
    right_fork_id = (philo->id + 1) % program->phil_count;

    pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
    *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
    write_monitor(program, philo);

    pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
    *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
    write_monitor(program, philo);

    update_meal_counter(philo);
    *(program->philosophers[philo->id].state) = EATING;
    write_monitor(program, philo);
  
}

void putdown_forks(t_program *program, t_philo *philo)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    int left_fork_id;
    int right_fork_id;

    left_fork_id = philo->id;
    right_fork_id = (philo->id + 1) % program->phil_count;
    
    pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);

    pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
   
    *(program->philosophers[philo->id].state) = SLEEPING;
    write_monitor(program, philo);

}


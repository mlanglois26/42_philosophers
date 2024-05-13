/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:10:57 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/13 18:24:22 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_for_all(t_program *program)
{
	pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	while (program->all_philo_created != 1)
	{
		pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
        precise_usleep(100000);
        pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	}
	pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
}


void fair_start(t_philo *philo)
{
    t_program *program;

    program = philo->ptr;

    if (program->phil_count % 2 == 0)
    {
        if (philo->id % 2 == 0)
        {
            pickup_forks(program, philo);
            // *(program->philosophers[philo->id].state) = EATING;
            precise_usleep(get_milli(program->time_to_eat));
            // write_monitor(program, philo);
            
            putdown_forks(program, philo);
            // *(program->philosophers[philo->id].state) = SLEEPING;
            precise_usleep(get_milli(program->time_to_sleep));
            // write_monitor(program, philo);
        
            *(program->philosophers[philo->id].state) = THINKING;
            write_monitor(program, philo);
        }
        else
            precise_usleep(1e4);
    }
    else
    {
        if (philo->id % 2 == 0)
        {
           precise_usleep(1e4);
        }
    }
}

void *thread_routine(void *data) 
{
    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;

    wait_for_all(program);
    
    fair_start(philo);
    while (1)
    {
        pickup_forks(program, philo);
        // *(program->philosophers[philo->id].state) = EATING;
        precise_usleep(get_milli(program->time_to_eat));
        // write_monitor(program, philo);
        
        putdown_forks(program, philo);
        // *(program->philosophers[philo->id].state) = SLEEPING;
        precise_usleep(get_milli(program->time_to_sleep));
	    // write_monitor(program, philo);
    
        *(program->philosophers[philo->id].state) = THINKING;
        write_monitor(program, philo);
    }

    return NULL;
}


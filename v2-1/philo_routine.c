/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:10:57 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/14 18:17:25 by malanglo         ###   ########.fr       */
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


void unlock_if_necessary(t_program *program, t_philo *philo)
{
    int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;
    
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        if (*(program->philosophers[i].state) == HAS_TAKEN_A_FORK)
            pthread_mutex_unlock(&program->forks_mutex[left_fork_id]) || pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
        if (*(program->philosophers[i].state) == EATING)
        {
            pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);    
            pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
        }
        i++;
    }

}




void *thread_routine(void *data)
{
    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;

    wait_for_all(program);

    
    if (philo->id % 2 == 0) 
    {
        precise_usleep(get_milli(program->time_to_eat));
    }
    else 
    {
        pickup_forks(program, philo);
        precise_usleep(get_milli(program->time_to_eat));

        putdown_forks(program, philo);

        precise_usleep(get_milli(program->time_to_sleep));

        pthread_mutex_lock(&program->monitor->philo_state_mutex);
        *(philo->state) = THINKING;
        pthread_mutex_unlock(&program->monitor->philo_state_mutex);

        pthread_mutex_lock(&program->monitor->access_mutex);
        write_monitor(program, philo);
        pthread_mutex_unlock(&program->monitor->access_mutex);
    }


    while (must_stop(program) != 1 || simulation_finished(program) == 1)
    {
       
        pthread_mutex_lock(&program->monitor->death_flag_mutex);
        int one_philo_died = program->one_philo_died;
        pthread_mutex_unlock(&program->monitor->death_flag_mutex);



        pickup_forks(program, philo);
        precise_usleep(get_milli(program->time_to_eat));

        pthread_mutex_lock(&program->monitor->death_flag_mutex);
        one_philo_died = program->one_philo_died;
        pthread_mutex_unlock(&program->monitor->death_flag_mutex);

        putdown_forks(program, philo);


        if (must_stop(program) == 1)
            break ;
            

        precise_usleep(get_milli(program->time_to_sleep));

        pthread_mutex_lock(&program->monitor->death_flag_mutex);
        one_philo_died = program->one_philo_died;
        pthread_mutex_unlock(&program->monitor->death_flag_mutex);
        
        

        if (must_stop(program) == 1)
            break ;
        
            
        pthread_mutex_lock(&program->monitor->philo_state_mutex);
        *(philo->state) = THINKING;
        pthread_mutex_unlock(&program->monitor->philo_state_mutex);

        pthread_mutex_lock(&program->monitor->access_mutex);
        write_monitor(program, philo);
        pthread_mutex_unlock(&program->monitor->access_mutex);


    }
    return NULL;
}



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:11:16 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/14 18:11:32 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



void update_meal_counter(t_philo *philo) 
{
    struct timeval	tv;

	t_program *program;

	program = philo->ptr;

	gettimeofday(&tv, NULL);
    
	pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
    philo->meal_counter++;
	philo->last_meal_time = (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
    pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
}

// dans actions pick up forks
void check_if_full(t_philo *philo)
{
    t_program *program;

    program = philo->ptr;
    
    pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
    if (philo->meal_counter == program->nb_of_time_to_eat)
        philo->full = 1;
    pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
}

// pour voir si j arrete mon dinner car nb of philo full reached
int new_nb_of_full_philos(t_program *program)
{
    int count;
    int i;
    
    count = 0;
    pthread_mutex_lock(&program->monitor->all_philos_full_mutex);
    i = 0;
    while (i < program->phil_count)
    {
        pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
        if (program->philosophers[i].full == 1)
            count++;
        pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
        i++;
    }
    pthread_mutex_unlock(&program->monitor->all_philos_full_mutex);
    return (count);
}

int simulation_finished(t_program *program)
{
    int res;
    
    res = 0;
    if (new_nb_of_full_philos(program) == program->phil_count)
        res = 1;
    return (res);
}
  









// void init_last_meal_time(t_program * program, t_philo)
// {
//     pthread_mutex_lock(&program->monitor->update_meal_counter_mutex); 
//     if (philo->last_meal_time == -1)
//         philo->last_meal_time = current_time;
//     pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex); 
// }
  
int philo_is_dead(t_philo *philo)
{
    int status;
    t_program *program;
    struct timeval tv;
    long current_time;
    
    status = 0;
    program = philo->ptr;
    
    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

    if (program->one_philo_died == 0)
    {
        pthread_mutex_lock(&program->monitor->update_meal_counter_mutex); 
        if (philo->last_meal_time == -1)
            philo->last_meal_time = current_time;
        pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex); 
    }
    
    pthread_mutex_lock(&program->monitor->update_meal_counter_mutex); 
    if ((current_time - philo->last_meal_time) > (program->time_to_die))
    {
        pthread_mutex_lock(&program->monitor->death_flag_mutex);
        program->one_philo_died = 1;
        pthread_mutex_unlock(&program->monitor->death_flag_mutex);
        status = 1;
    }
    pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex); 

    return (status);
}

int must_stop(t_program *program)
{
    int res;
    
    res = 0;
    pthread_mutex_lock(&program->monitor->death_flag_mutex);
    if (program->one_philo_died == 1)
        res = 1;
    pthread_mutex_unlock(&program->monitor->death_flag_mutex);
    return (res);
}



// void unlock_if_necessary(t_program *program, t_philo *philo) {
    
//     t_program *program = (t_program *)philo->ptr;
    
//     int left_fork_id = philo->id;
//     int right_fork_id = (philo->id + 1) % program->phil_count;

//     // Unlock the forks held by this philosopher
//     pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
//     pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
// }

void *monitor_routine(void *data) 
{
    // long test;
    // test = (long)data; // Cast du pointeur data à son type d'origine (long)

    t_program *program;
    int i;
    
    program = (t_program *)data;
  
    wait_for_all(program);
    // pthread_mutex_lock(&program->protection_mutex);
    while (must_stop(program) != 1)
    {
        i = 0;
        while (i < program->phil_count && program->end_of_program_flag == 0)
        {
            if (philo_is_dead(&program->philosophers[i]) == 1)
            {
                pthread_mutex_lock(&program->monitor->philo_state_mutex);
                *(program->philosophers[i].state) = DEAD; 
                pthread_mutex_unlock(&program->monitor->philo_state_mutex);
        
                pthread_mutex_lock(&program->monitor->access_mutex);
                write_monitor(program, &program->philosophers[i]);
                pthread_mutex_unlock(&program->monitor->access_mutex);
                
                // unlock_if_necessary(program, &program->philosophers[i]);
                pthread_mutex_lock(&program->monitor->death_flag_mutex);
                program->end_of_program_flag = 1; 
                pthread_mutex_unlock(&program->monitor->death_flag_mutex);
            }
            i++;
        }
    }
    // pthread_mutex_unlock(&program->protection_mutex);


    return NULL;
}

// int death_occured(t_program *program)
// {
//     int res;
//     int i;

//     res = 0;
//     while (program->end_of_program_flag == 0)
//     {
//         i = 0;
//         while (i < program->phil_count)
//         {
//             if (philo_is_dead(&program->philosophers[i]) == 1)
//                 {
//                     printf("res = %d\n", res);
//                     program->end_of_program_flag = 1;
//                     res = 1;
                    
//                 }
//                 i++;
//         }
//     }
    
//     return (res);
// }




// void unlock_if_necessary(void *data)
// {
//     t_philo *philo = (t_philo *)data;
//     t_program *program = (t_program *)philo->ptr;
    
//     int	left_fork_id;
// 	int	right_fork_id;

// 	left_fork_id = philo->id;
// 	right_fork_id = (philo->id + 1) % program->phil_count;
    
//     int i;

//     i = 0;
//     while (i < program->phil_count)
//     {
//         if (*(program->philosophers[i].state) == EATING)
//         {
//             pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);    
//             pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
//         }
//         if (*(program->philosophers[i].state) == HAS_TAKEN_A_FORK)
//             pthread_mutex_unlock(&program->forks_mutex[left_fork_id]) || pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
//         i++;
//     }

// }


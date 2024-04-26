/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:55:12 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/26 15:52:52 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void write_monitor(t_program *program, t_philo *philo)
{
    int state;
    struct timeval tv;
    long clock;

    pthread_mutex_lock(program->printf_mutex);
    state = (*(philo->state));
    gettimeofday(&tv, NULL);
    clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    pthread_mutex_unlock(program->printf_mutex);

    if (state == THINKING)
        printf("%ld %d is thinking\n", clock, philo->id + 1);
    else if (state == HAS_TAKEN_A_FORK)
        printf("%ld %d has taken a fork\n", clock, philo->id + 1);
    else if (state == EATING)
        printf("%ld %d is eating\n", clock, philo->id + 1);
    else if (state == SLEEPING)
        printf("%ld %d is sleeping\n", clock, philo->id + 1);
    else if (state == DEAD)
    {
        printf("%ld %d is dead\n", clock, philo->id + 1);
        exit (0);
    }
    else
        printf("Dinner is over\n");
}

int is_valid_set_up(t_program *program)
{
    if (program->phil_count % 2 == 0)
    {
        if (program->philosophers->time_to_die >= (program->philosophers->time_to_eat + program->philosophers->time_to_sleep + 10))
            return (1);
        else
        {
            printf("Wrong set up. With EVEN nb of philos time_to_die >= (time_to_eat + time_to_sleep + 10)\n");
            return (0);
        }
            
    }
    else if (program->phil_count % 2 != 0)
    {
        if (program->philosophers->time_to_die >= program->philosophers->time_to_eat * 2 + program->philosophers->time_to_sleep + 10)
            return (1);
        else
        {
            printf("Wrong set up. With ODD nb of philos time_to_die >= (time_to_eat * 2 + time_to_sleep + 10)\n");
            return (0);
        }
    }
    else
        return (0);
}

void fair_start(t_philo *philo)
{
    t_program *program;

    program = philo->ptr;
    
    if (program->phil_count % 2 == 0)
    {
        if (philo->id % 2 != 0)
        {
            precise_usleep(philo->time_to_eat);
            *(program->philosophers[philo->id].state) = THINKING;
            write_monitor(program, philo);
        }
        else
        {
            pickup_forks(program, philo);
            precise_usleep(get_milli(philo->time_to_eat));

            putdown_forks(program, philo);
            precise_usleep(get_milli(philo->time_to_sleep));

            *(program->philosophers[philo->id].state) = THINKING;
            write_monitor(program, philo);
        }
    }
    // else
    // {
    //     if (philo->id % 2 == 0)
    //     {
    //        precise_usleep(1e4);
    //     }
    // }
}

// je dois faire en sorte de commencer avec l id 1 et pas 0

void *thread_routine(void *data) 
{
    long philo_birth;

    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;
    
    wait_for_all(program);
    
    philo_birth = program->start_of_program;

    philo->last_meal_time = philo_birth;

    fair_start(philo);
    
    if (is_valid_set_up(program) == 1)
    {
        // if (philo->id % 2 != 0)
        // {
        //     printf("philo nb %d\n", philo->id);
        //     precise_usleep(philo->time_to_eat);
        // }
    
        fair_start(philo);
        
        while (all_full(program) == 0 && end_of_program(program) == 0)
        {
            // if (end_of_program(program) == 1)
            //     break ; 
            
            if (can_i_eat(program, philo) == 1)
            {
                pickup_forks(program, philo);
                precise_usleep(get_milli(philo->time_to_eat));

                putdown_forks(program, philo);
                precise_usleep(get_milli(philo->time_to_sleep));

                if (end_of_program(program) == 1)
                    break ; 

                *(program->philosophers[philo->id].state) = THINKING;
                write_monitor(program, philo);
            }
            
        }   
    }

    
    return NULL;
}


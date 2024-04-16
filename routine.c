/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:55:12 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/16 16:10:17 by malanglo         ###   ########.fr       */
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
        printf("%ld %d is thinking\n", clock, philo->id);
    else if (state == HAS_TAKEN_A_FORK)
        printf("%ld %d has taken a fork\n", clock, philo->id);
    else if (state == EATING)
        printf("%ld %d is eating\n", clock, philo->id);
    else if (state == SLEEPING)
        printf("%ld %d is sleeping\n", clock, philo->id);
    else if (state == DEAD)
        printf("%ld %d is dead\n", clock, philo->id);
    else
        printf("Dinner is over\n");
}

void *thread_routine(void *data) 
{
    long philo_birth;

    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;
    
    wait_for_all(program);
    
    philo_birth = program->start_of_program;

    philo->last_meal_time = philo_birth;

    if (philo->id % 2 == 0)
        usleep(1000);

    while (end_of_program(program) == 0)
    {
        pickup_forks(program, philo);
        usleep(get_milli(philo->time_to_eat));

        putdown_forks(program, philo);
        usleep(get_milli(philo->time_to_sleep));

        *(program->philosophers[philo->id].state) = THINKING;
        write_monitor(program, philo);
    }
    return NULL;
}

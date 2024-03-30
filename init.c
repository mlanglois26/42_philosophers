/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 10:14:28 by malanglo          #+#    #+#             */
/*   Updated: 2024/03/30 11:35:08 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void init_forks(t_program *program)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        pthread_mutex_init(&program->forks_mutex[i], NULL);
        i++; 
    }
}

void init_philosophers(t_program *program)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        program->philosophers[i].id = i + 1;
        program->philosophers[i].phil_count = program->phil_count;
        program->philosophers[i].life_starting_time = program->start_of_program;
        program->philosophers[i].time_to_die = 0;
        program->philosophers[i].time_to_eat = 0;
        program->philosophers[i].time_to_sleep = 0;
        program->philosophers[i].meal_count = 0;
        program->philosophers[i].ptr = program;
        
        program->philosophers[i].last_meal_time = malloc(sizeof(long));
        program->philosophers[i].blocktime = malloc(sizeof(long));
        program->philosophers[i].state = malloc(sizeof(int));
        i++;
    }
}

void init_program(t_program *program, struct timeval *tv)
{
    program->start_of_program = tv->tv_usec;
    
    program->philosophers = malloc(sizeof(t_philo) * program->phil_count);
    if (!program->philosophers)
        return (NULL);
    init_philosophers(program);
    
    program->forks_mutex = malloc(sizeof(pthread_mutex_t) * program->phil_count);
    if (!program->forks_mutex)
        return (NULL);
    init_forks(program);
}

void fill_philo_struct(t_philo *philo, char **argv)
{
    if (valid_args(**argv) == 1)
    {
        philo->phil_count = ft_atol(argv[1]);
        philo->time_to_die = ft_atol(argv[2]);
        philo->time_to_eat = ft_atol(argv[3]);
        philo->time_to_sleep = ft_atol(argv[4]);
        if (argv[5])
            philo->meal_count = ft_atol(argv[5]);
        else
            philo->meal_count = -1;
    }
}

// if (philo->time_to_die < 6e4)
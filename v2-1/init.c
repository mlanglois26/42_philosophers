/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:42:24 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/14 17:20:27 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void init_philo_struct(t_program *program)
{
    int i;
    int mutex;

    i = 0;
    while (i < program->phil_count)
    {
        program->philosophers[i].id = i;
        program->philosophers[i].ptr = program;
        program->philosophers[i].has_been_created = 0;
        program->philosophers[i].meal_counter = 0;
        program->philosophers[i].last_meal_time = -1;
        program->philosophers[i].full = 0;
        program->philosophers[i].is_dead = 0;
        program->philosophers[i].state = malloc(sizeof(int));
        *(program->philosophers[i].state) = THINKING;
        mutex = pthread_mutex_init(&program->philosophers[i].full_mutex, NULL);
        if (mutex != 0)
            printf("error full_mutex has been created mutex failed\n");
        i++;
    }
}

void init_forks(t_program *program)
{
    int i;
    int res;

    i = 0;
    while (i < program->phil_count)
    {
        res = pthread_mutex_init(&program->forks_mutex[i], NULL);
        i++; 
    }
    i = 0;
    while (i < program->phil_count)
    {
        if (res != 0)
            printf("error en creant fork mutex %d\n", i);
        i++;
    }
}

t_program *malloc_program(char **argv)
{   
    t_program *program;
    
    program = malloc(sizeof(t_program));
    if (!program)
        return (NULL);
    program->monitor = malloc(sizeof(t_monitor));
    if (!program->monitor)
    {
        free(program);
        return (NULL);
    }
    program->phil_count = ft_atol(argv[1]);
    program->philosophers = malloc(sizeof(t_philo) * program->phil_count);
    if (!program->philosophers)
    {
        free(program->monitor);
        free(program);
        return (NULL);
    }
    program->forks_mutex = malloc(sizeof(pthread_mutex_t) * program->phil_count);
    if (!program->forks_mutex)
    {
        free(program->philosophers);
        free(program->monitor);
        free(program);
        return (NULL);
    }
    return (program);
}

void init_program(t_program *program, char **argv)
{
    program->all_philo_created = 0;
    program->all_full = 0;
    program->start_of_program = 0;
    program->end_of_program_flag = 0;
    program->one_philo_died = 0;
    program->time_to_die = ft_atol(argv[2]);
    program->time_to_eat = ft_atol(argv[3]);
    program->time_to_sleep = ft_atol(argv[4]);
    if (argv[5])
        program->nb_of_time_to_eat = ft_atol(argv[5]);
    else
        program->nb_of_time_to_eat = -1;
}

void init_program_mutex(t_program *program)
{
    init_forks(program);

    int res11 = pthread_mutex_init(&program->shared_data_mutex, NULL);
    if (res11 != 0)
        printf("error mutex pour shared_data failed\n");
    else
        printf("mutex pour shared data a bien ete cree\n");
    
    int res4 = pthread_mutex_init(&program->one_philo_has_been_created_mutex, NULL);
        if (res4 != 0)
        printf("error one philo has been created mutex failed\n");
    else
        printf("one philo has been created mutex a bien ete cree\n");
    
    int res2 = pthread_mutex_init(&program->all_philos_have_been_created_mutex, NULL);
    if (res2 != 0)
        printf("error mutex pour all_philos_have_been_created_mutex_mutex failed\n");
    else
        printf("mutex pour all_philos_have_been_created_mutex_mutex a bien ete cree\n");
    int res = pthread_mutex_init(&program->printf_mutex, NULL);
    if (res != 0)
        printf("error mutex pour printf failed\n");
    else
        printf("mutex pour printf a bien ete cree\n");
    
    int res69 = pthread_mutex_init(&program->end_of_program_mutex, NULL);
    if (res69 != 0)
        printf("error end_of_prog has been created mutex failed\n");
    else
        printf("end_of_prog has been created mutex a bien ete cree\n");

     int res6 = pthread_mutex_init(&program->protection_mutex, NULL);
    if (res6 != 0)
        printf("error protection_mutex has been created mutex failed\n");
    else
        printf("protection_mutex has been created mutex a bien ete cree\n");
    
}

void init_monitor(t_program *program)
{ 
    int res1 = pthread_mutex_init(&program->monitor->one_philo_is_full_mutex, NULL);
        if (res1 != 0)
        printf("error one philo is full mutex failed\n");
    else
        printf("one philo is full mutex a bien ete cree\n");

    int res2 = pthread_mutex_init(&program->monitor->all_philos_full_mutex, NULL);
    if (res2 != 0)
        printf("error mutex pour all_philos_full_mutex_mutex failed\n");
    else
        printf("mutex pour all_philos_full_mutex_mutex a bien ete cree\n");

    int res11 = pthread_mutex_init(&program->monitor->update_meal_counter_mutex, NULL);
    if (res11 != 0)
        printf("error mutex pour update_meal_counter_mutex_mutex failed\n");
    else
        printf("mutex pour all_philos_have_been_created_mutex_mutex a bien ete cree\n");

    int res12 = pthread_mutex_init(&program->monitor->philo_state_mutex, NULL);
    if (res12 != 0)
        printf("error mutex pour state_mutex_mutex failed\n");
    else
        printf("mutex pour state_mutex a bien ete cree\n");

    int res13 = pthread_mutex_init(&program->monitor->access_mutex, NULL);
    if (res13 != 0)
        printf("error mutex pour access_mitex failed\n");
    else
        printf("mutex pour access_mutex a bien ete cree\n");

     int res14 = pthread_mutex_init(&program->monitor->death_flag_mutex, NULL);
    if (res14 != 0)
        printf("error death_falg_mutex failed\n");
    else
        printf("mutex pour death_flag_mutex a bien ete cree\n");
}

// void init_program_mutex(t_program *program)
// {
//     init_forks(program);

//     pthread_mutex_init(&program->shared_data_mutex, NULL);
//     pthread_mutex_init(&program->one_philo_has_been_created_mutex, NULL);
//     pthread_mutex_init(&program->all_philos_have_been_created_mutex, NULL);
//     pthread_mutex_init(&program->printf_mutex, NULL);
//     pthread_mutex_init(&program->end_of_program_mutex, NULL);
// }

// void init_monitor(t_program *program)
// { 
//     pthread_mutex_init(&program->monitor->one_philo_is_full_mutex, NULL);
//     pthread_mutex_init(&program->monitor->all_philos_full_mutex, NULL);
//     pthread_mutex_init(&program->monitor->update_meal_counter_mutex, NULL);
//     pthread_mutex_init(&program->monitor->philo_state_mutex, NULL);
//     pthread_mutex_init(&program->monitor->access_mutex, NULL);
// }

// t_program *set_dinner_data(char **argv)
// {
//     t_program *program;
   
//     program = malloc_program(argv);
//     if (!program) {
//         printf("Failed to allocate memory for program\n");
//         exit(EXIT_FAILURE);
//     }
    
//     init_program(program, argv);
//     init_program_mutex(program);
//     init_philo_struct(program);
//     init_monitor(program);
//     return program;
// }


t_program *set_dinner_data(char **argv)
{
    t_program *program;
   
    program = malloc_program(argv);
    if (!program)
        return (NULL);
    init_program(program, argv);
    init_program_mutex(program);
    init_philo_struct(program);  
    init_monitor(program);
    return (program);
}











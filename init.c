/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:26:12 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/08 18:30:08 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void handle_philo_mutexes(t_program *program, int i)
{
    int res_1;
    int res_2;
    
    res_1 = pthread_mutex_init(&program->philosophers[i].update_meal_counter_mutex, NULL);
    if (res_1 != 0)
        printf("error meal_counter_mutex has been created mutex failed\n");
    res_2 = pthread_mutex_init(&program->philosophers[i].full_mutex, NULL);
    if (res_2 != 0)
        printf("error full_mutex has been created mutex failed\n");
}

void fill_philo_struct(t_program *program, char **argv)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        program->philosophers[i].id = i;
        program->philosophers[i].ptr = program;
        program->philosophers[i].has_been_created = 0;
        program->philosophers[i].meal_counter = 0;
        program->philosophers[i].last_meal_time = -1;
        program->philosophers[i].philo_birth = -1;
        program->philosophers[i].full = 0;
        program->philosophers[i].is_dead = 0;
        program->philosophers[i].state = malloc(sizeof(int));
        program->philosophers[i].time_to_die = ft_atol(argv[2]);
        program->philosophers[i].time_to_eat = ft_atol(argv[3]);
        program->philosophers[i].time_to_sleep = ft_atol(argv[4]);
        if (argv[5])
            program->philosophers[i].nb_of_time_to_eat = ft_atol(argv[5]);
        else
            program->philosophers[i].nb_of_time_to_eat = -1;
        handle_philo_mutexes(program, i);
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

t_program *handle_program_mutexes(t_program *program)
{
//    int res_3 = pthread_mutex_init(&program->test, NULL);
//     if (res_3 != 0)
//         printf("error test_mutex has been created mutex failed\n");

//     int res_30 = pthread_mutex_init(&program->global_mutex, NULL);
//     if (res_30 != 0)
//         printf("error global_mutex_mutex has been created mutex failed\n");
   
//    int res11 = pthread_mutex_init(&program->update_meal_counter_mutex, NULL);
//     if (res11 != 0)
//         printf("error mutex pour update_meal_counter_mutex_mutex failed\n");
//     else
//         printf("mutex pour all_philos_have_been_created_mutex_mutex a bien ete cree\n");
    
    int res2 = pthread_mutex_init(&program->all_philos_have_been_created_mutex, NULL);
    if (res2 != 0)
        printf("error mutex pour all_philos_have_been_created_mutex_mutex failed\n");
    else
        printf("mutex pour all_philos_have_been_created_mutex_mutex a bien ete cree\n");
    
    int res4 = pthread_mutex_init(&program->one_philo_has_been_created_mutex, NULL);
        if (res4 != 0)
        printf("error one philo has been created mutex failed\n");
    else
        printf("one philo has been created mutex a bien ete cree\n");

    int res22 = pthread_mutex_init(&program->one_philo_is_full_mutex, NULL);
    if (res22 != 0)
        printf("error mutex pour onephiloisfuul_mutex failed\n");
    else
        printf("mutex pour onephiloisfuul_mutex a bien ete cree\n");
    
    int res24 = pthread_mutex_init(&program->all_philo_full_mutex, NULL);
        if (res24 != 0)
        printf("error one philo has been created mutex failed\n");
    else
        printf("one philo has been created mutex a bien ete cree\n");

    // int res6 = pthread_mutex_init(&program->nb_of_full_philo_mutex, NULL);
    //     if (res6 != 0)
    //     printf("error nb_of_full_philo_mutex has been created mutex failed\n");
    // else
    //     printf("nb_of_full_philo_mutex has been created mutex a bien ete cree\n");

    // int res16 = pthread_mutex_init(&program->nb_of_full_philo_mutex_2, NULL);
    //     if (res16 != 0)
    //     printf("error nb_of_full_philo_mutex has been created mutex failed\n");
    // else
    //     printf("nb_of_full_philo_mutex has been created mutex a bien ete cree\n");
    
    program->printf_mutex = malloc(sizeof(pthread_mutex_t));
    if (!program->printf_mutex)
    {
        free(program->philosophers);
        free(program->forks_mutex);
        free(program);
        return (NULL);
    }

    int res = pthread_mutex_init(program->printf_mutex, NULL);
    if (res != 0)
        printf("error mutex pour printf failed\n");
    else
        printf("mutex pour printf a bien ete cree\n");
    
    // int res7 = pthread_mutex_init(&program->all_full_mutex, NULL);
    // if (res7 != 0)
    //     printf("error mutex pour printf failed\n");
    // else
    //     printf("mutex pour printf a bien ete cree\n");

    int res9 = pthread_mutex_init(&program->check_for_death_mutex, NULL);
        if (res9 != 0)
            printf("error check_for_death_mutex has been created mutex failed\n");
        else
            printf("check_for_death_mutex has been created mutex a bien ete cree\n");

    int res29 = pthread_mutex_init(&program->clock_mutex, NULL);
    if (res29 != 0)
        printf("error clock_mutex has been created mutex failed\n");
    else
        printf("clock_mutex has been created mutex a bien ete cree\n");

    int res39 = pthread_mutex_init(&program->protection_mutex, NULL);
    if (res39 != 0)
        printf("error protection_mutex has been created mutex failed\n");
    else
        printf("protection_mutex has been created mutex a bien ete cree\n");

     int res49 = pthread_mutex_init(&program->other, NULL);
    if (res49 != 0)
        printf("error other has been created mutex failed\n");
    else
        printf("other has been created mutex a bien ete cree\n");
    
    
    // int res10 = pthread_mutex_init(&program->stop_mutex, NULL);
    // if (res10 != 0)
    //     printf("error stop_mutex has been created mutex failed\n");
    // else
    //     printf("stop_mutex has been created mutex a bien ete cree\n");
    return (program);
}

t_program *init_program(char **argv)
{   
    t_program *program;
    
    program = malloc(sizeof(t_program));
    if (!program)
        return (NULL);
    program->phil_count = ft_atol(argv[1]);
    program->all_philo_created = 0;
    program->all_full = 0;
    program->start_of_program = 0;
    program->end_of_program_flag = 0;
    program->philosophers = malloc(sizeof(t_philo) * program->phil_count);
    if (!program->philosophers)
    {
        free(program);
        return (NULL);
    }
    program->forks_mutex = malloc(sizeof(pthread_mutex_t) * program->phil_count);
    if (!program->forks_mutex)
    {
        free(program->philosophers);
        free(program);
        return (NULL);
    }
    init_forks(program);
    fill_philo_struct(program, argv);
    return (program);
}




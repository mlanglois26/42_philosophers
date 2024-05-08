/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:52:53 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/08 17:45:32 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void clean_program(t_program *program)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        pthread_mutex_destroy(&program->forks_mutex[i]);
        pthread_mutex_destroy(&program->philosophers[i].update_meal_counter_mutex);
        pthread_mutex_destroy(&program->philosophers[i].full_mutex);
        free(program->philosophers[i].state);
        i++;
    }
    pthread_mutex_destroy(&program->one_philo_has_been_created_mutex);
    pthread_mutex_destroy(&program->all_philos_have_been_created_mutex);
    pthread_mutex_destroy(&program->one_philo_is_full_mutex);
    pthread_mutex_destroy(&program->all_philo_full_mutex);
    // pthread_mutex_destroy(&program->nb_of_full_philo_mutex);
    // pthread_mutex_destroy(&program->all_full_mutex);
    // pthread_mutex_destroy(&program->check_for_death_mutex);
    pthread_mutex_destroy(program->printf_mutex);
    pthread_mutex_destroy(&program->protection_mutex);
    free(program->printf_mutex);
    free(program->forks_mutex);
    free(program->philosophers);
    free(program);
}


void print_philo_struct(t_philo *philo) 
{
    printf("philo id: %d\n", philo->id);
    printf("time to die: %ld\n", philo->time_to_die);
    printf("time to eat: %ld\n", philo->time_to_eat);
    printf("time to sleep: %ld\n", philo->time_to_sleep);
    printf("nb_of_time_to_eat: %d\n", philo->nb_of_time_to_eat);
    printf("philo has been created = %d\n", philo->has_been_created);
}

int	main(int argc, char **argv)
{
    t_program *program;
  
    if (verif_args(argc, argv) == 1)
        printf("all good with args\n");
    else
    {
        printf("pb with args\n");
        exit (0);
    }
    
    program = init_program(argv);
    program = handle_program_mutexes(program);

    int i = 0; 
    while (i < program->phil_count) 
    {
        printf("\nPhilosopher %d:\n", i + 1);
        print_philo_struct(&program->philosophers[i]);
        i++;
    }
    printf("\n\n");
    
    handle_threads(program);
    clean_program(program);
	return (0);
}

// pb avec la mort
// pb du cas ou meal count == 0 - probablement faire 2 fonctions pour la mort et checker fill_philo_struct
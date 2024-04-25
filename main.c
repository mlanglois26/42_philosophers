/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:52:53 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/25 13:14:04 by malanglo         ###   ########.fr       */
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
    pthread_mutex_destroy(&program->nb_of_full_philo_mutex);
    pthread_mutex_destroy(&program->all_full_mutex);
    pthread_mutex_destroy(&program->check_for_death_mutex);
    pthread_mutex_destroy(program->printf_mutex);
    free(program->printf_mutex);
    free(program->forks_mutex);
    free(program->philosophers);
    free(program);
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
    
    handle_threads(program);
    clean_program(program);
	return (0);
}

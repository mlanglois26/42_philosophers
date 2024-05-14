/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:12:03 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/14 18:25:58 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void print_philo_struct(t_philo *philo) 
{
    printf("philo id: %d\n", philo->id);
    printf("philo has been created = %d\n", philo->has_been_created);
    printf("philo meal counter = %d\n", philo->meal_counter);
    printf("philo full ? = %d\n", philo->full);
    printf("philo last meal time = %ld\n", philo->last_meal_time);
    printf("philo is dead ? = %d\n", philo->is_dead);
    printf("philo state = %d\n\n", *(philo->state));
}

void printf_program(t_program *program)
{
    printf("Program address: %p\n", (void *)program);
    printf("Program data:\n");
    printf("phil_count: %d\n", program->phil_count);
    printf("time_to_die: %ld\n", program->time_to_die);
    printf("time_to_eat: %ld\n", program->time_to_eat);
    printf("time_to_sleep: %ld\n", program->time_to_sleep);
    printf("nb_of_time_to_eat: %d\n", program->nb_of_time_to_eat);
    printf("all philo created ?: %d\n", program->all_philo_created);
    printf("all full ?: %d\n", program->all_full);
    printf("start of program: %ld\n", program->start_of_program);
    printf("end of program: %d\n\n", program->end_of_program_flag);


}

void print_forks(t_program *program) {
    printf("Fork mutexes:\n");
    for (int i = 0; i < program->phil_count; i++) {
        int status = pthread_mutex_trylock(&program->forks_mutex[i]);
        if (status == 0) {
            printf("Fork %d is available\n", i);
            pthread_mutex_unlock(&program->forks_mutex[i]); // Déverrouiller le mutex après l'avoir vérifié
        // } else if (status == EBUSY) {
        //     printf("Fork %d is busy\n", i);
        } else {
            printf("Error checking fork %d\n", i);
        }
    }
}

void clean_program(t_program *program)
{
    if (!program) return;

    int i;

    // Destroy mutexes for each philosopher's fork and state
    for (i = 0; i < program->phil_count; i++)
    {
        pthread_mutex_destroy(&program->forks_mutex[i]);
        pthread_mutex_destroy(&program->philosophers[i].full_mutex);
        free(program->philosophers[i].state);
    }

    // Destroy program-level mutexes
    pthread_mutex_destroy(&program->shared_data_mutex);
    pthread_mutex_destroy(&program->one_philo_has_been_created_mutex);
    pthread_mutex_destroy(&program->all_philos_have_been_created_mutex);
    pthread_mutex_destroy(&program->printf_mutex);
    pthread_mutex_destroy(&program->end_of_program_mutex);

    // Destroy monitor mutexes
    if (program->monitor) {
        pthread_mutex_destroy(&program->monitor->one_philo_is_full_mutex);
        pthread_mutex_destroy(&program->monitor->all_philos_full_mutex);
        pthread_mutex_destroy(&program->monitor->update_meal_counter_mutex);
        pthread_mutex_destroy(&program->monitor->philo_state_mutex);
        pthread_mutex_destroy(&program->monitor->access_mutex);
        pthread_mutex_destroy(&program->monitor->death_flag_mutex);
        free(program->monitor);
    }

    // Free dynamically allocated memory
    free(program->forks_mutex);
    free(program->philosophers);
    free(program);
}

// void clean_program(t_program *program)
// {
//     int i;

//     i = 0;
//     while (i < program->phil_count)
//     {
//         pthread_mutex_destroy(&program->forks_mutex[i]);
//         pthread_mutex_destroy(&program->philosophers[i].full_mutex);
//         free(program->philosophers[i].state);
//         i++;
//     }
//     pthread_mutex_destroy(&program->one_philo_has_been_created_mutex);
//     pthread_mutex_destroy(&program->all_philos_have_been_created_mutex);
//     free(program->forks_mutex);
//     free(program->monitor);
//     free(program->philosophers);
//     free(program);
// }

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

    program = set_dinner_data(argv);
    
    int i = 0;
    while (i < program->phil_count)
    {
        print_philo_struct(&program->philosophers[i]);
        i++;
    }
    print_forks(program);
    handle_threads(program);
    clean_program(program);
	return (0);
}


// de temps en temps philo meurt avec 410 donc pas normal 
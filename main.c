/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 10:15:41 by malanglo          #+#    #+#             */
/*   Updated: 2024/03/29 16:48:07 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//     // wait for all threads to be Created
//     // starts dinner according to nb of philo
//     // check if one philo dies or if dinner is over

// void *monitor(t_program *program, t_philo *philo)
// {
//     pthread_mutex_lock(program->monitor);
//     while (philo->phil_count != program->phil_count)
//         ;
//     pthread_mutex_unlock(program->monitor);
//     return (NULL);
// }

// int end_of_program(t_philo *philo)
// {
//     struct timeval current_time;
    
//     gettimeofday(&current_time, NULL);
    
//     if (philo->last_meal_time == 0)
//         *(philo->last_meal_time) = (long)philo->life_starting_time;
    
//     if (((*philo->last_meal_time) - (current_time.tv_usec)) > (philo->time_to_die - philo->time_to_eat))
//     {
//         printf("philo %d died\n", philo->id);
//         return (1);
//     }
//     // else if (meal_count reach)
//     return (0);
// }

// void script_monitor(t_program *program, t_philo *philo)
// {
//     int state;
//     struct timeval tv;

//     pthread_mutex_lock(program->script);
//     state = (*(philo->state));
//     long res = gettimeofday(&tv, NULL);
//     pthread_mutex_unlock(program->script);

//     if (state == THINKING)
//         printf("%ld %d is thinking\n", tv.tv_usec, philo->id);
//     else if (state == EATING)
//         printf("%ld %d is eating\n", tv.tv_usec, philo->id);
//     else if (state == SLEEPING)
//         printf("%ld %d is sleeping\n", tv.tv_usec, philo->id);
//     else if (state == DEAD)
//     {
//         printf("%ld %d is dead\n", tv.tv_usec, philo->id);
//         // exit (0);
//     }
//     //  else if ()
//     //     printf("Philo %d has taken a fork\n", philo->id);
//     else
//     {
//         printf("%ld Dinner is over\n", tv.tv_usec);
//         exit (0);
//     }
// }

// void start_dinner_according_to_philo_position(t_program *program, t_philo *philo)
// {
//     if (philo->id % 2 != 0 && (philo->id != program->phil_count))
//         usleep(philo->time_to_eat / 2);
// }

// void *thread_routine(void *data)
// {
//     t_philo *philo;
//     t_program *program;

//     philo = (t_philo *)data;
//     program = (t_program *)philo->ptr;;
   
//     monitor(program, philo);
//     start_dinner_according_to_philo_position(program, philo);
//     while (end_of_program(philo) != 1)
//     {
//         printf("coucou\n");
//         pickup_forks(program, philo);
//         *(program->philosophers[philo->id].state) = EATING;
//         // script_monitor(program, philo);
//         // precise_usleep(philo->time_to_eat);
//     }
//     return (NULL);
// }



// void create_philo_threads(t_program *program)
// {
//     int i;
//     int ret;

//     i = 0;
//     while (i < program->phil_count)
//     {
//         ret = pthread_create(&program->philosophers[i].thread_id, NULL, thread_routine, (void *)&program->philosophers[i]);
//         if (ret != 0)
// 		{
// 			printf("Erreur lors de la creation des threads\n");
// 			exit(0);
// 		}
//         i++;
//     }
//     i = 0;
//     while (i < program->phil_count)
// 	{
// 		ret = pthread_join(program->philosophers[i].thread_id, NULL);
// 		if (ret != 0)
// 		{
// 			printf("Erreur lors de la jonction des threads\n");
// 			exit(0);
// 		}
// 		i++;
// 	}
// }

int	main(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] and if wanted [nb_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
    
    t_program program;
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    init_program(&program, &tv);
	
	init_philosophers(&program, nb_of_philosophers, time_to_die, time_to_eat, time_to_sleep);
    
    fill_data_input(philo, argv);

	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:55:12 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/09 15:55:44 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void write_monitor(t_program *program, t_philo *philo)
{
    int state;
    struct timeval tv;
    long clock;

    pthread_mutex_lock(&program->printf_mutex);
    pthread_mutex_lock(&program->philo_state_mutex); // Verrouillage de l'accès à la variable state
    // state = (*(philo->state));
    state = *(program->philosophers[philo->id].state);
    pthread_mutex_unlock(&program->philo_state_mutex); // Déverrouillage de l'accès à la variable state
    gettimeofday(&tv, NULL);
    clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

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
        // exit (0);
    }
    else
        printf("Dinner is over\n");
    pthread_mutex_unlock(&program->printf_mutex);
}


// void write_monitor(t_program *program, t_philo *philo)
// {
//     int state;
//     struct timeval tv;
//     long clock;

//     pthread_mutex_lock(program->printf_mutex);
//     state = (*(philo->state));
//     gettimeofday(&tv, NULL);
//     clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

//     if (state == THINKING)
//         printf("%ld %d is thinking\n", clock, philo->id + 1);
//     else if (state == HAS_TAKEN_A_FORK)
//         printf("%ld %d has taken a fork\n", clock, philo->id + 1);
//     else if (state == EATING)
//         printf("%ld %d is eating\n", clock, philo->id + 1);
//     else if (state == SLEEPING)
//         printf("%ld %d is sleeping\n", clock, philo->id + 1);
//     else if (state == DEAD)
//     {
//         printf("%ld %d is dead\n", clock, philo->id + 1);
//     }
//     else
//         printf("Dinner is over\n");
//     pthread_mutex_unlock(program->printf_mutex);
// }

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
        if (philo->id % 2 == 0)
        {
            pickup_forks(program, philo);
            precise_usleep(get_milli(philo->time_to_eat));

            putdown_forks(program, philo);
            precise_usleep(get_milli(philo->time_to_sleep));

            *(program->philosophers[philo->id].state) = THINKING;
            write_monitor(program, philo);
        }
        else
            precise_usleep(1e4);
    }
    else
    {
        if (philo->id % 2 == 0)
        {
           precise_usleep(1e4);
        }
    }
}


// void *thread_routine(void *data) 
// {

//     t_philo *philo = (t_philo *)data;
//     t_program *program = (t_program *)philo->ptr;
    
//     wait_for_all(program);
//     // fair_start(philo);
    
//     while (!simulation_finished(program))
//     { 
//         if (new_nb_of_full_philos(program) == program->phil_count)
//             break ;
        
//         pickup_forks(program, philo);
//         precise_usleep(get_milli(philo->time_to_eat));

//         putdown_forks(program, philo);
//         precise_usleep(get_milli(philo->time_to_sleep));
        
//         pthread_mutex_lock(&program->philo_state_mutex);
//         *(program->philosophers[philo->id].state) = THINKING;
//         pthread_mutex_unlock(&program->philo_state_mutex);
//         write_monitor(program, philo);
//     }
//     printf("simulation finished\n");
//     return NULL;
// }

 // / pickup_forks(program, philo);
            // precise_usleep(get_milli(philo->time_to_eat));

            // putdown_forks(program, philo);
            // precise_usleep(get_milli(philo->time_to_sleep));

            // *(program->philosophers[philo->id].state) = THINKING;
            // write_monitor(program, philo);
    //    }
        // else
        // {
        //     precise_usleep(philo->time_to_eat);
        //     *(program->philosophers[philo->id].state) = THINKING;
        //     write_monitor(program, philo);
        // }

// je dois faire en sorte de commencer avec l id 1 et pas 0

void *thread_routine(void *data) 
{
    // long philo_birth;

    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;

    wait_for_all(program);
    
    // philo_birth = program->start_of_program;
    // printf("philo birth = %ld\n", philo->philo_birth);
    
    // philo->last_meal_time = philo_birth;
    
    if (is_valid_set_up(program) == 1)
    {   
        // if (philo->nb_of_time_to_eat == 0)
        // {
        //     *(program->philosophers[philo->id].state) = THINKING;
        //     write_monitor(program, philo);
        //     precise_usleep(get_milli(philo->time_to_die)); 
            
        //     *(program->philosophers[philo->id].state) = DEAD;
        //     write_monitor(program, philo);
            
        //     // if (*(program->philosophers[philo->id].state) == DEAD)
        //     // {
        //     //     program->end_of_program_flag = 1;
        //     //     test_clean(program);
        //     // }
        // }
        // else if (program->phil_count == 1)
        // {
        //     *(program->philosophers[philo->id].state) = THINKING;
        //     write_monitor(program, philo);
        //     precise_usleep(get_milli(philo->time_to_die)); 
            
            
        //     // *(program->philosophers[philo->id].state) = DEAD;
        //     // write_monitor(program, philo);
        // }
        // else
        // {
            // fair_start(philo);
           
            while (*philo->state != DEAD && simulation_finished(program) != 1)
            { 
                
                // printf("hellooooo\n");
                // if (philo->nb_of_time_to_eat == 0)
                // {
                //     *(program->philosophers[philo->id].state) = THINKING;
                //     write_monitor(program, philo);
                //     precise_usleep(get_milli(philo->time_to_die)); 
                    
                //     *(program->philosophers[philo->id].state) = DEAD;
                //     write_monitor(program, philo);
                //     philo->is_dead = 1;
                // }
                if (program->phil_count == 1)
                {
                    if (philo->last_meal_time == -1)
                        philo->last_meal_time = program->start_of_program;
                    pthread_mutex_lock(&program->philo_state_mutex);
                    *(program->philosophers[philo->id].state) = THINKING;
                     pthread_mutex_unlock(&program->philo_state_mutex);
                    write_monitor(program, philo);
                    precise_usleep(get_milli(philo->time_to_die + 1)); 
                }
                else
                {
                    if (new_nb_of_full_philos(program) == program->phil_count)
                        break ;
           
                    if (simulation_finished(program) == 1)
                        break ;
                    
                    pickup_forks(program, philo);
                    precise_usleep(get_milli(philo->time_to_eat));

                    // if (simulation_finished(program) == 1)
                    //     break ;
                    if (program->end_of_program_flag == 1)
                        break ; 
 
                    putdown_forks(program, philo);
                    precise_usleep(get_milli(philo->time_to_sleep));
             
                   *(program->philosophers[philo->id].state) = THINKING;
                    pthread_mutex_lock(&program->access_mutex);
                    write_monitor(program, philo);
                    pthread_mutex_unlock(&program->access_mutex);
                }
            }   
        // }
    }
    return NULL;
}



  // pthread_mutex_lock(&program->clock_mutex);
                // // struct timeval tv;
                // // gettimeofday(&tv, NULL);
                // // long clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

                // if (philo_is_dead(philo) == 1)
                //     printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiia\n");
                // pthread_mutex_unlock(&program->clock_mutex);
    // if (end_of_program(program) == 1)
                //     break ; 
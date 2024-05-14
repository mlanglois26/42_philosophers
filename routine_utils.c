/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:00:49 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/14 10:48:02 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ici j attends que le flag all_created passe a 1 avant de start routine
// (le flag est mis a 1 dans wait_for_everybody_and_set_starting_time())

void	wait_for_all(t_program *program)
{
	pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	while (program->all_philo_created != 1)
	{
		pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
		precise_usleep(100000);
		pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	}
	pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
}

// dans actions pick up forks
void check_if_full(t_philo *philo)
{
    t_program *program;

    program = philo->ptr;
    
    pthread_mutex_lock(&program->new_update_meal_counter_mutex);
    if (philo->meal_counter == philo->nb_of_time_to_eat)
        philo->full = 1;
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex);
}

// pour voir si j arrete mon dinner car nb of philo full reached
int new_nb_of_full_philos(t_program *program)
{
    int count;
    int i;
    
    count = 0;
    pthread_mutex_lock(&program->all_philo_full_mutex);
    i = 0;
    while (i < program->phil_count)
    {
        pthread_mutex_lock(&program->new_update_meal_counter_mutex);
        if (program->philosophers[i].full == 1)
            count++;
        pthread_mutex_unlock(&program->new_update_meal_counter_mutex);
        i++;
    }
    pthread_mutex_unlock(&program->all_philo_full_mutex);
    return (count);
}

    // pthread_mutex_lock(&philo->update_meal_counter_mutex); 
    // if (philo->last_meal_time == -1)
    //     philo->last_meal_time = current_time;
    // pthread_mutex_unlock(&philo->update_meal_counter_mutex); 


void check_if_philo_has_locked_a_fork(t_philo * philo)
{
    if (*philo->state == HAS_TAKEN_A_FORK)
    {
        while (*philo->state == HAS_TAKEN_A_FORK)
           // unlock mutex
        return (0);   
    }
    if (*philo->state == EATING)
    {
        while (*philo->state == EATING)
           // unlock mutex
        return (0);   
    }
    return (0);
}
    
int philo_is_dead(t_philo *philo)
{
    int status;
    t_program *program;
    struct timeval tv;
    long current_time;
    
    status = 0;
    program = philo->ptr;
    
    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

    pthread_mutex_lock(&program->new_update_meal_counter_mutex); 
    if (philo->last_meal_time == -1)
        philo->last_meal_time = current_time;
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex); 
        
    pthread_mutex_lock(&program->check_for_death_mutex);
    pthread_mutex_lock(&program->new_update_meal_counter_mutex);
  
    if ((current_time - philo->last_meal_time) > (philo->time_to_die))
    {
        if (*philo->state != HAS_TAKEN_A_FORK)
        {
            printf("ok pb ici -- philo nb  | le philo etait en meme tempt en train de vouloir acceder qu write monitor je pense il faut proteger le state et le write %d\n", philo->id + 1);
        }
           if (*philo->state != EATING)
        {
            printf("another pb ici -- philo nb %d\n", philo->id + 1);
        }
        
        pthread_mutex_lock(&program->philo_state_mutex);
        *(program->philosophers[philo->id].state) = DEAD; 
        pthread_mutex_unlock(&program->philo_state_mutex);
        pthread_mutex_lock(&program->access_mutex);
        write_monitor(program, philo);
        pthread_mutex_unlock(&program->access_mutex);
        status = 1;
        // printf("philo is deadddddddddddd\n");
    }
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex);
    pthread_mutex_unlock(&program->check_for_death_mutex);

    return status;
}

void *monitor_program(void *data)
{
    t_program *program;
    
    program = (t_program *)data;

    wait_for_all(program);

    while (!simulation_finished(program))
    {
        int i = 0;
        while (i < program->phil_count && !simulation_finished(program))
        {    
            if (philo_is_dead(&program->philosophers[i]) == 1)
            {
                // printf("info transmise\n");
                pthread_mutex_lock(&program->protection_mutex);
                program->end_of_program_flag = 1;
                // printf("flag leve\n");
                pthread_mutex_unlock(&program->protection_mutex);
            }
            i++;
        }
    }
    return (NULL);
}


int simulation_finished(t_program *program)
{
    int res;
    
    pthread_mutex_lock(&program->protection_mutex);
    res = program->end_of_program_flag;
    // printf("res= %d\n", res);
    pthread_mutex_unlock(&program->protection_mutex);
    return (res);
}



    
int end_of_program(t_program *program)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {    
        if (philo_is_dead(&program->philosophers[i]) == 1)
        {
            return 1;
        }
        i++;
    }
    return 0;
}


// int philo_is_dead(t_philo *philo)
// {
//     int status;
//     t_program *program;
//     struct timeval tv;
//     long current_time;
    
//     status = 0;
//     program = philo->ptr;
    
//     pthread_mutex_lock(&program->check_for_death_mutex);

//     gettimeofday(&tv, NULL);
//     current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    
//     if (current_time - philo->last_meal_time > philo->time_to_die)
//     {
//         philo.state = DEAD;
//         status = 1;
//     }
//     pthread_mutex_unlock(&program->check_for_death_mutex);

//     return (status);
// }



// int all_full(t_program *program)
// {
//     pthread_mutex_lock(&program->stop_mutex);
//     int result = 0;
//     if (nb_of_full_philos(program) == program->phil_count)
//     {
//         printf("FUUUUUUUUUUUUUUULL\n");
//         result = 1;
//         exit (0);
//     }
//     pthread_mutex_unlock(&program->stop_mutex);
//     return result;
// }



// int philo_is_dead(t_program *program)
// {
//     t_philo *philo;
//     struct timeval tv;
//     long current_time;
//     int status;
//     int i;
    
//     pthread_mutex_lock(&program->check_for_death_mutex);

//     gettimeofday(&tv, NULL);
//     current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    
//     status = 0;
//     i = 0;
//     while (i < program->phil_count)
//     {
      
   
//         philo = &program->philosophers[i];
        
//         if (philo->last_meal_time == -1)
//             philo->last_meal_time = current_time;
       
//         if ((current_time - philo->last_meal_time) > program->philosophers->time_to_die)
//         {
//             *(program->philosophers[philo->id].state) = DEAD;
//             status = 1;
//             break ;
//         }  
//         else
//             status = 0;
//         i++;
//     }
    
//     pthread_mutex_unlock(&program->check_for_death_mutex);
//     if (status == 1)
//         return (1);
//     else
//         return (0);
// }


// int philo_is_dead(t_program *program)
// {
//     // printf("entering philo_is_dead\n");
//     t_philo *philo;
//     struct timeval tv;
//     long current_time;
//     int status;
//     int i;
//     // int res = 0;
//     // printf("start of programme = %ld\n\n", program->start_of_program);
//     pthread_mutex_lock(&program->check_for_death_mutex);

//     gettimeofday(&tv, NULL);
//     current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
//     // printf("\n\nCURRENT time = %ld\n\n", current_time);
//     status = 0;
//     i = 0;
//     while (i < program->phil_count)
//     {
//         // printf("current time = %ld\n", current_time);
//         // printf("philo last meal time = %ld\n", philo->last_meal_time);
//         // printf("curremt time = %ld\n", current_time);
//         // printf("program philo time to die = %ld\n", program->philosophers->time_to_die);
//         // printf("res = %ld | %ld\n", (current_time - philo->last_meal_time), (current_time - program->philosophers->time_to_die));
   
//         philo = &program->philosophers[i];
        
//         if (philo->last_meal_time == -1)
//             philo->last_meal_time = current_time;
//         // if (res > program->philosophers->time_to_die)
//         //     status = 1;
        
//         // si last_time meal == -1 
//         // le pb vient d ici start current time - start_of program
//         if ((current_time - philo->last_meal_time) > program->philosophers->time_to_die)
//         {
//             // printf("CURRENT time = %ld\n", current_time);
//             // printf("philo last meal time = %ld\n", philo->last_meal_time);
//             // printf("curremt time = %ld\n", current_time);
//             // printf("program philo time to die = %ld\n", program->philosophers->time_to_die);
//             // printf("res = %ld | %ld\n", (current_time - philo->last_meal_time), (current_time - program->philosophers->time_to_die));
//             // printf("check philo %d died\n", i);
//             *(program->philosophers[philo->id].state) = DEAD;
//             printf("%ld %d is dead\n", current_time, philo->id);
//             status = 1;
//             break ;
//         }  
//         else
//             status = 0;
//         i++;
//     }
    
//     pthread_mutex_unlock(&program->check_for_death_mutex);
//     if (status == 1)
//         return (1);
//     else
//         return (0);
// }


// int	nb_of_full_philos(t_program *program)
// {
// 	t_philo *philo;
// 	int count;
// 	int i;

// 	count = 0;
// 	i = 0;
// 	pthread_mutex_lock(&program->new_nb_of_full_philo_mutex);
// 	while (i < program->phil_count)
// 	{
// 		philo = &program->philosophers[i];
// 		if (philo->meal_counter >= philo->nb_of_time_to_eat)
// 			count++;
// 		i++;
// 	}
// 	pthread_mutex_unlock(&program->new_nb_of_full_philo_mutex);
// 	return (count);
// }

// int all_full(t_program *program)
// {
//    int res;

// //     pthread_mutex_lock(&program->nb_of_full_philo_mutex_2);
//    res = 0;
//    if (new_nb_of_full_philos(program) == program->phil_count)
//     {
//         // printf("FUUUUUUUUUUUUUUULL\n");
//         res = 1;
//         exit (0);
//     }
//     // pthread_mutex_unlock(&program->nb_of_full_philo_mutex_2);
//     return (res);
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:00:49 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/16 16:09:08 by malanglo         ###   ########.fr       */
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

// pour voir si j arrete mon dinner car nb of meal reached

int	nb_of_full_philos(t_program *program)
{
	t_philo *philo;
	int count;
	int i;

	count = 0;
	i = 0;
	pthread_mutex_lock(&program->nb_of_full_philo_mutex);
	while (i < program->phil_count)
	{
		philo = &program->philosophers[i];
		if (philo->meal_counter >= philo->nb_of_time_to_eat)
			count++;
		i++;
	}
	pthread_mutex_unlock(&program->nb_of_full_philo_mutex);
	return (count);
}


int philo_is_dead(t_program *program)
{
    t_philo *philo;
    struct timeval tv;
    long current_time;
    int status;
    int i;

    pthread_mutex_lock(&program->check_for_death_mutex);

    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    i = 0;
    while (i < program->phil_count)
    {
        philo = &program->philosophers[i];
        if ((current_time - philo->last_meal_time) > (current_time - program->philosophers->time_to_die))
        {
            if (philo->last_meal_time == -1)
                break ;
            else
            {
                printf("current time = %ld\n", current_time);
                printf("philo last meal time = %ld\n", philo->last_meal_time);
                printf("curremt time = %ld\n", current_time);
                printf("program philo time to die = %ld\n", program->philosophers->time_to_die);
                printf("res = %ld | %ld\n", (current_time - philo->last_meal_time), (current_time - program->philosophers->time_to_die));
                printf("philo %d died\n", philo->id);
                status = 1;
                break ;
            }
        }  
        else
            status = 0;
        i++;
    }
    
    pthread_mutex_unlock(&program->check_for_death_mutex);
    return (status);
}


int end_of_program(t_program *program)
{
    // printf("\n\nentering end of prgram\n");
    if (nb_of_full_philos(program) == program->phil_count)
        return (1);
    if (philo_is_dead(program) == 1)
    {
        printf("one philo died\n");
        return (1);
    }
    else
        return (0);
}
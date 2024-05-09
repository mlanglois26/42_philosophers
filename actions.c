/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:48:09 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/09 13:49:21 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



// il faut que je bascule ca ici

// pthread_mutex_lock(&philo->update_meal_counter_mutex); 
// if (philo->last_meal_time == -1)
// 	philo->last_meal_time = current_time;
// pthread_mutex_unlock(&philo->update_meal_counter_mutex); 

void update_meal_counter(t_philo *philo) 
{
    struct timeval	tv;

	t_program *program;
	// long current_time;

	program = philo->ptr;

	// int i = 0;
	// gettimeofday(&tv, NULL);
    // current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
	// while (i < program->phil_count)
	// {
	// 	pthread_mutex_lock(&program->new_update_meal_counter_mutex); 
	// 	if (program->philosophers[i].last_meal_time == -1)
	// 		program->philosophers[i].last_meal_time = current_time;
	// 	pthread_mutex_unlock(&program->new_update_meal_counter_mutex); 
	// 	i++;
	// }
	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&program->new_update_meal_counter_mutex);
    philo->meal_counter++;
	philo->last_meal_time = (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex);
}

void	pickup_forks(t_program *program, t_philo *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;
	
	if (simulation_finished(program) == 0)
	{
		if ((philo->id + 1) - program->phil_count == 0)
		{
			pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
			
			// pthread_mutex_lock(&program->philo_state_mutex);
			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
			// pthread_mutex_unlock(&program->philo_state_mutex);
			
			// pthread_mutex_lock(&program->access_mutex);
			write_monitor(program, philo);
			// pthread_mutex_unlock(&program->access_mutex);
	
			pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
			
			// pthread_mutex_lock(&program->philo_state_mutex);
			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
			// pthread_mutex_unlock(&program->philo_state_mutex);
			
			// pthread_mutex_lock(&program->access_mutex);
			write_monitor(program, philo);
			// pthread_mutex_unlock(&program->access_mutex);
		}
		else
		{
			pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
			
			pthread_mutex_lock(&program->philo_state_mutex);
			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
			pthread_mutex_unlock(&program->philo_state_mutex);
			
			pthread_mutex_lock(&program->access_mutex);
			write_monitor(program, philo);
			pthread_mutex_unlock(&program->access_mutex);
			
			pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
			
			pthread_mutex_lock(&program->philo_state_mutex);
			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
			pthread_mutex_unlock(&program->philo_state_mutex);
			
			pthread_mutex_lock(&program->access_mutex);
			write_monitor(program, philo);
			pthread_mutex_unlock(&program->access_mutex);
		}
		update_meal_counter(philo);
		check_if_full(philo);
		pthread_mutex_lock(&program->philo_state_mutex);
		*(program->philosophers[philo->id].state) = EATING;
		pthread_mutex_unlock(&program->philo_state_mutex);
		write_monitor(program, philo);
	}
	else
		printf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n");


}


void	putdown_forks(t_program *program, t_philo *philo)
{
	struct timeval	tv;
	int				left_fork_id;
	int				right_fork_id;

	gettimeofday(&tv, NULL);
	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;

	if ((philo->id + 1) - program->phil_count == 0)
	{
		pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
		pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
	}
	else
	{
		pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
		pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
	}
	pthread_mutex_lock(&program->philo_state_mutex);
	*(program->philosophers[philo->id].state) = SLEEPING;
	pthread_mutex_unlock(&program->philo_state_mutex);
	write_monitor(program, philo);

}


// if (philo_is_dead(philo) == 1)
//         {
// 			*(program->philosophers[philo->id].state) = DEAD;
//         	write_monitor(program, philo);
// 		}


// int	can_i_eat(t_program *program, t_philo *philo)
// {
// 	int	g;
// 	int	d;

// 	g = (philo->id + program->phil_count - 1) % program->phil_count;
// 	d = (philo->id + 1) % program->phil_count;
// 	if (program->philosophers[g].last_meal_time < philo->last_meal_time
// 		&& program->philosophers[d].last_meal_time < philo->last_meal_time)
// 		return (0);
// 	else
// 		return (1);
// }

// apres avoir pris les 2 forks
// void	update_meal_counter(t_philo *philo)
// {
// 	struct timeval	tv;
// 	t_program *program;

//     program = philo->ptr;
// 	gettimeofday(&tv, NULL);
// 	pthread_mutex_lock(&program->update_meal_counter_mutex);
// 	philo->meal_counter++;
// 	philo->last_meal_time = (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
// 	pthread_mutex_unlock(&program->update_meal_counter_mutex);
// }
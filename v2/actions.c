/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:58:13 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/13 18:23:47 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void write_monitor(t_program *program, t_philo *philo)
{
    int state;
    struct timeval tv;
    long clock;

    pthread_mutex_lock(&program->printf_mutex);
    
    state = *(program->philosophers[philo->id].state);
    gettimeofday(&tv, NULL);
    clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    
    if (state == DEAD)
    {
        printf("%ld %d is dead\n", clock, philo->id);
        // exit (0);
    }
  
    if (state == THINKING)
        printf("%ld %d is thinking\n", clock, philo->id);
    if (state == HAS_TAKEN_A_FORK)
        printf("%ld %d has taken a fork\n", clock, philo->id);
    if (state == EATING)
        printf("%ld %d is eating\n", clock, philo->id);
    if (state == SLEEPING)
        printf("%ld %d is sleeping\n", clock, philo->id);
   
    pthread_mutex_unlock(&program->printf_mutex);

}

// int is_last_philo(t_program *program, t_philo *philo)
// {
//     if ((philo->id) - program->phil_count == 0)
//         return (1);
//     return (0);
// }

// void take_right_fork_first(t_program *program, t_philo *philo, int right_fork_id, int left_fork_id)
// {
//     pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
//     *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
    
//     write_monitor(program, philo);

//     pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
//     *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
     
//     write_monitor(program, philo);
// }

// void take_left_fork_first(t_program *program, t_philo *philo, int left_fork_id, int right_fork_id)
// {
//     pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
//     *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK; 
//     printf("left fork = %d\n", left_fork_id);
//     write_monitor(program, philo);
        
//     pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
//     *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
//     printf("right_fork = %d\n", right_fork_id);
//     write_monitor(program, philo);
// }


// void	pickup_forks(t_program *program, t_philo *philo)
// {
// 	int	left_fork_id;
// 	int	right_fork_id;

// 	left_fork_id = philo->id;
// 	right_fork_id = (philo->id + 1) % program->phil_count;
	
//     if (is_last_philo(program, philo))
//         take_right_fork_first(program, philo, right_fork_id, left_fork_id);
//     else
//         take_left_fork_first(program, philo, left_fork_id, right_fork_id);
    
//     // update_meal_counter(philo);
//     // check_if_full(philo);
//     // *(program->philosophers[philo->id].state) = EATING;
//     // precise_usleep(get_milli(program->time_to_eat));
//     // write_monitor(program, philo);
// }

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
	
    *(program->philosophers[philo->id].state) = SLEEPING;
    // precise_usleep(get_milli(program->time_to_sleep));
	write_monitor(program, philo);
}



void	pickup_forks(t_program *program, t_philo *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;

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
			
			// pthread_mutex_lock(&program->philo_state_mutex);
			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
			// pthread_mutex_unlock(&program->philo_state_mutex);
			
			// pthread_mutex_lock(&program->access_mutex);
			write_monitor(program, philo);
			// pthread_mutex_unlock(&program->access_mutex);
			
			pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
			
			// pthread_mutex_lock(&program->philo_state_mutex);
			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
			// pthread_mutex_unlock(&program->philo_state_mutex);
			
			// pthread_mutex_lock(&program->access_mutex);
			write_monitor(program, philo);
			// pthread_mutex_unlock(&program->access_mutex);
		}
		// update_meal_counter(philo);
		// check_if_full(philo);
		// pthread_mutex_lock(&program->philo_state_mutex);
		*(program->philosophers[philo->id].state) = EATING;
		// pthread_mutex_unlock(&program->philo_state_mutex);
		// pthread_mutex_lock(&program->access_mutex);
		write_monitor(program, philo);
		// pthread_mutex_unlock(&program->access_mutex);



}
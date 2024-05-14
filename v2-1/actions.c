/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:58:13 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/14 18:23:58 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void write_monitor(t_program *program, t_philo *philo)
// {
//     int state;
//     struct timeval tv;
//     long clock;

//     // pthread_mutex_lock(&program->printf_mutex);
    
//     state = *(program->philosophers[philo->id].state);
//     gettimeofday(&tv, NULL);
//     clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    
//     if (state == DEAD)
//     {
//         printf("%ld %d is dead\n", clock, philo->id);
//         // exit (0);
//     }
  
//     if (state == THINKING)
//         printf("%ld %d is thinking\n", clock, philo->id);
//     if (state == HAS_TAKEN_A_FORK)
//         printf("%ld %d has taken a fork\n", clock, philo->id);
//     if (state == EATING)
//         printf("%ld %d is eating\n", clock, philo->id);
//     if (state == SLEEPING)
//         printf("%ld %d is sleeping\n", clock, philo->id);
   
//     // pthread_mutex_unlock(&program->printf_mutex);

// }

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
    
// 	pthread_mutex_lock(&program->printf_mutex);
//     write_monitor(program, philo);
// 	pthread_mutex_unlock(&program->printf_mutex);

//     pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
//     *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
    
// 	pthread_mutex_lock(&program->printf_mutex);
//     write_monitor(program, philo);
// 	pthread_mutex_unlock(&program->printf_mutex);
// }

// void take_left_fork_first(t_program *program, t_philo *philo, int left_fork_id, int right_fork_id)
// {
//     pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
//     *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK; 
//     // printf("left fork = %d\n", left_fork_id);
// 	pthread_mutex_lock(&program->printf_mutex);
//     write_monitor(program, philo);
// 	pthread_mutex_unlock(&program->printf_mutex);
        
//     pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
//     *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
//     // printf("right_fork = %d\n", right_fork_id);
// 	pthread_mutex_lock(&program->printf_mutex);
//     write_monitor(program, philo);
// 	pthread_mutex_unlock(&program->printf_mutex);
// }


// void	pickup_forks(t_program *program, t_philo *philo)
// {
// 	int	left_fork_id;
// 	int	right_fork_id;

// 	left_fork_id = philo->id;
// 	right_fork_id = (philo->id + 1) % program->phil_count;
	
//     if ((philo->id + 1) - program->phil_count == 0)
// 	{
// 		take_right_fork_first(program, philo, right_fork_id, left_fork_id);
// 	}
//     else
// 	{
// 		take_left_fork_first(program, philo, left_fork_id, right_fork_id);
// 	}
    
//     // update_meal_counter(philo);
//     // check_if_full(philo);
//     *(program->philosophers[philo->id].state) = EATING;
//     // precise_usleep(get_milli(program->time_to_eat));
// 	pthread_mutex_lock(&program->printf_mutex);
//     write_monitor(program, philo);
// 	pthread_mutex_unlock(&program->printf_mutex);
// }

// void	putdown_forks(t_program *program, t_philo *philo)
// {
// 	struct timeval	tv;
// 	int				left_fork_id;
// 	int				right_fork_id;

// 	gettimeofday(&tv, NULL);
// 	left_fork_id = philo->id;
// 	right_fork_id = (philo->id + 1) % program->phil_count;

// 	if ((philo->id + 1) - program->phil_count == 0)
// 	{
// 		pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
// 		pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
// 	}
// 	else
// 	{
// 		pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
// 		pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
// 	}
	
//     *(program->philosophers[philo->id].state) = SLEEPING;
//     // precise_usleep(get_milli(program->time_to_sleep));
// 	pthread_mutex_lock(&program->printf_mutex);
// 	write_monitor(program, philo);
// 	pthread_mutex_unlock(&program->printf_mutex);
// }



// void write_monitor(t_program *program, t_philo *philo)
// {
//     int state;
//     struct timeval tv;
//     long clock;

//     pthread_mutex_lock(&program->printf_mutex);

// 	pthread_mutex_lock(&program->monitor->philo_state_mutex);
//     state = *(program->philosophers[philo->id].state);
//     pthread_mutex_unlock(&program->monitor->philo_state_mutex);
    
// 	gettimeofday(&tv, NULL);
//     clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

//     if (state == THINKING)
//         printf("%ld %d is thinking\n", clock, philo->id);
//     if (state == HAS_TAKEN_A_FORK)
//         printf("%ld %d has taken a fork\n", clock, philo->id);
//     if (state == EATING)
//         printf("%ld %d is eating\n", clock, philo->id);
//     if (state == SLEEPING)
//         printf("%ld %d is sleeping\n", clock, philo->id);
//     if (state == DEAD)
//         printf("%ld %d is dead\n", clock, philo->id);
//     pthread_mutex_unlock(&program->printf_mutex);
// }

void write_monitor(t_program *program, t_philo *philo)
{
    int state;
    struct timeval tv;
    long clock;

	
    pthread_mutex_lock(&program->printf_mutex);

    pthread_mutex_lock(&program->monitor->philo_state_mutex);
    if (philo->state == NULL) {
        pthread_mutex_unlock(&program->monitor->philo_state_mutex);
        pthread_mutex_unlock(&program->printf_mutex);
        return;
    }
    state = *(philo->state);
    pthread_mutex_unlock(&program->monitor->philo_state_mutex);
	pthread_mutex_unlock(&program->printf_mutex);

    
    gettimeofday(&tv, NULL);
    clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
	
	if (state == DEAD)
        printf("%ld %d is dead\n", clock, philo->id);
	// else if (must_stop(program) == 1)
    // 	printf("qqqqqqqqqqqqqqqqqq\n");

	if (must_stop(program) == 1)
    {
        // printf("qqqqqqqqqqqqqqqqqq\n");
        return; // Exit the function early
    }
	
    if (state == THINKING)
        printf("%ld %d is thinking\n", clock, philo->id);
    else if (state == HAS_TAKEN_A_FORK)
        printf("%ld %d has taken a fork\n", clock, philo->id);
    else if (state == EATING)
		printf("%ld %d is eating\n", clock, philo->id);
    else if (state == SLEEPING)
		printf("%ld %d is sleeping\n", clock, philo->id);

}




void	pickup_forks(t_program *program, t_philo *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;
	
	if (must_stop(program) == 1)
            return ;	

	if ((philo->id + 1) - program->phil_count == 0)
	{
		pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
		
		pthread_mutex_lock(&program->monitor->philo_state_mutex);
		*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
		pthread_mutex_unlock(&program->monitor->philo_state_mutex);
		
		pthread_mutex_lock(&program->monitor->access_mutex);
		write_monitor(program, philo);
		pthread_mutex_unlock(&program->monitor->access_mutex);
	

		pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
		
		pthread_mutex_lock(&program->monitor->philo_state_mutex);
		*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
		pthread_mutex_unlock(&program->monitor->philo_state_mutex);
		
		pthread_mutex_lock(&program->monitor->access_mutex);
		write_monitor(program, philo);
		pthread_mutex_unlock(&program->monitor->access_mutex);
		
	}
	else
	{
		pthread_mutex_lock(&program->forks_mutex[left_fork_id]);

		pthread_mutex_lock(&program->monitor->philo_state_mutex);
		*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
		pthread_mutex_unlock(&program->monitor->philo_state_mutex);
		
		pthread_mutex_lock(&program->monitor->access_mutex);
		write_monitor(program, philo);
		pthread_mutex_unlock(&program->monitor->access_mutex);
		
		pthread_mutex_lock(&program->forks_mutex[right_fork_id]);

		pthread_mutex_lock(&program->monitor->philo_state_mutex);
		*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
		pthread_mutex_unlock(&program->monitor->philo_state_mutex);
		
		pthread_mutex_lock(&program->monitor->access_mutex);
		write_monitor(program, philo);
		pthread_mutex_unlock(&program->monitor->access_mutex);
	}
	
	pthread_mutex_lock(&program->monitor->death_flag_mutex);
	int any_philo_dead = program->one_philo_died;
	pthread_mutex_unlock(&program->monitor->death_flag_mutex);

	if (!any_philo_dead && !philo_is_dead(philo)) {
		
        update_meal_counter(philo);
        check_if_full(philo);

        pthread_mutex_lock(&program->monitor->philo_state_mutex);
        *(program->philosophers[philo->id].state) = EATING;
        pthread_mutex_unlock(&program->monitor->philo_state_mutex);

        pthread_mutex_lock(&program->monitor->access_mutex);
        write_monitor(program, philo);
        pthread_mutex_unlock(&program->monitor->access_mutex);
    }
	
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

	if (must_stop(program) == 1)
            return ;	

	pthread_mutex_lock(&program->monitor->philo_state_mutex);
	*(program->philosophers[philo->id].state) = SLEEPING;
	pthread_mutex_unlock(&program->monitor->philo_state_mutex);
	
	pthread_mutex_lock(&program->monitor->access_mutex);
	write_monitor(program, philo);
	pthread_mutex_unlock(&program->monitor->access_mutex);
}


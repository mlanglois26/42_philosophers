/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:58:13 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/23 09:57:01 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_write_monitor(t_program *program, t_philo *philo)
{
	pthread_mutex_lock(&program->monitor->access_mutex);
	write_monitor(program, philo);
	pthread_mutex_unlock(&program->monitor->access_mutex);
}

void	write_monitor(t_program *program, t_philo *philo)
{
	int				state;
	long			clock;
	struct timeval	tv;

	pthread_mutex_lock(&program->monitor->philo_state_mutex);
	state = *(philo->state);
	pthread_mutex_unlock(&program->monitor->philo_state_mutex);
	gettimeofday(&tv, NULL);
	clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
	pthread_mutex_lock(&program->monitor->printf_mutex);
	if (state == DEAD)
		printf("%ld %d died\n", clock, philo->id + 1);
	pthread_mutex_unlock(&program->monitor->printf_mutex);
	if (death_alert(program) == 1)
		return ;
	pthread_mutex_lock(&program->monitor->printf_mutex);
	if (state == THINKING)
		printf("%ld %d is thinking\n", clock, philo->id + 1);
	else if (state == HAS_TAKEN_A_FORK)
		printf("%ld %d has taken a fork\n", clock, philo->id + 1);
	else if (state == EATING)
		printf("%ld %d is eating\n", clock, philo->id + 1);
	else if (state == SLEEPING)
		printf("%ld %d is sleeping\n", clock, philo->id + 1);
	pthread_mutex_unlock(&program->monitor->printf_mutex);
}

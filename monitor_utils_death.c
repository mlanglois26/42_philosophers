/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils_death.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:25:51 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:33:17 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_is_dead(t_philo *philo)
{
	int				status;
	t_program		*program;
	struct timeval	tv;
	long			current_time;

	status = 0;
	program = philo->ptr;
	gettimeofday(&tv, NULL);
	current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
	pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
	if (philo->last_meal_time == -1)
		philo->last_meal_time = current_time;
	pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
	pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
	if ((current_time - philo->last_meal_time) > (program->time_to_die))
	{
		pthread_mutex_lock(&program->monitor->death_flag_mutex);
		program->one_philo_died = 1;
		pthread_mutex_unlock(&program->monitor->death_flag_mutex);
		status = 1;
	}
	pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
	return (status);
}

int	death_alert(t_program *program)
{
	int	res;

	res = 0;
	pthread_mutex_lock(&program->monitor->death_flag_mutex);
	if (program->one_philo_died == 1)
		res = 1;
	pthread_mutex_unlock(&program->monitor->death_flag_mutex);
	return (res);
}

int	get_death_flag_value(t_program *program)
{
	int	res;

	pthread_mutex_lock(&program->monitor->death_flag_mutex);
	res = program->one_philo_died;
	pthread_mutex_unlock(&program->monitor->death_flag_mutex);
	return (res);
}

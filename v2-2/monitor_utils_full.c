/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils_full.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 09:48:56 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:33:54 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_meal_counter(t_philo *philo)
{
	struct timeval	tv;
	t_program		*program;

	program = philo->ptr;
	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
	philo->meal_counter++;
	philo->last_meal_time = (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
	pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
}

// dans actions.c pick up forks
void	check_if_full(t_philo *philo)
{
	t_program	*program;

	program = philo->ptr;
	pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
	if (philo->meal_counter == program->nb_of_time_to_eat)
		philo->full = 1;
	pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
}

// pour voir si j arrete mon dinner car nb of philo full reached
int	nb_of_full_philos(t_program *program)
{
	int	count;
	int	i;

	count = 0;
	pthread_mutex_lock(&program->monitor->all_philos_full_mutex);
	i = 0;
	while (i < program->phil_count)
	{
		pthread_mutex_lock(&program->monitor->update_meal_counter_mutex);
		if (program->philosophers[i].full == 1)
			count++;
		pthread_mutex_unlock(&program->monitor->update_meal_counter_mutex);
		i++;
	}
	pthread_mutex_unlock(&program->monitor->all_philos_full_mutex);
	return (count);
}

int	everybody_is_full(t_program *program)
{
	int	res;

	res = 0;
	if (nb_of_full_philos(program) >= program->phil_count)
		res = 1;
	return (res);
}

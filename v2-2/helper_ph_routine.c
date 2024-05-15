/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_ph_routine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:16:14 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:20:14 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_program *program, t_philo *philo)
{
	update_meal_counter(philo);
	check_if_full(philo);
	set_philo_state(program, philo, EATING);
	safe_write_monitor(program, philo);
	precise_usleep(get_milli(program->time_to_eat));
}

void	think_accordingly(t_program *program, t_philo *philo)
{
	if (program->phil_count % 2 == 0)
	{
		set_philo_state(program, philo, THINKING);
		safe_write_monitor(program, philo);
	}
	else
	{
		set_philo_state(program, philo, THINKING);
		safe_write_monitor(program, philo);
		precise_usleep(get_milli((program->time_to_eat * 2
					- program->time_to_sleep) * 0.42));
	}
}

void	set_philo_state(t_program *program, t_philo *philo, int assign_state)
{
	pthread_mutex_lock(&program->monitor->philo_state_mutex);
	*(program->philosophers[philo->id].state) = assign_state;
	pthread_mutex_unlock(&program->monitor->philo_state_mutex);
}

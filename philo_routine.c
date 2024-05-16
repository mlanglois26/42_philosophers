/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:10:57 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/16 11:45:46 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_for_all(t_program *program)
{
	pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	while (program->all_philo_created != 1)
	{
		pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
		precise_usleep(1000);
		pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	}
	pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
}

static void	fair_start(t_program *program, t_philo *philo)
{
	if (philo->id % 2 == 0)
		precise_usleep(get_milli(program->time_to_eat));
}

static void	special_routine(t_program *program, t_philo *philo)
{
	set_philo_state(program, philo, THINKING);
	safe_write_monitor(program, philo);
	precise_usleep(get_milli(program->time_to_die));
	if (death_alert(program) == 1)
		return ;
}

void	sleep_and_think(t_program *program, t_philo *philo)
{
	putdown_forks(program, philo);
	if (death_alert(program) == 1)
		return ;
	think_accordingly(program, philo);
}

void	*thread_routine(void *data)
{
	t_philo		*philo;
	t_program	*program;
	int			is_there_a_dead_philo;

	philo = (t_philo *)data;
	program = (t_program *)philo->ptr;
	wait_for_all(program);
	fair_start(program, philo);
	if (program->nb_of_time_to_eat == 0 || program->phil_count == 1)
		special_routine(program, philo);
	else
	{
		while (death_alert(program) != 1 && everybody_is_full(program) != 1)
		{
			if (death_alert(program) == 1)
				break ;
			pickup_forks(program, philo);
			is_there_a_dead_philo = get_death_flag_value(program);
			if (is_there_a_dead_philo == 0)
				eat(program, philo);
			sleep_and_think(program, philo);
		}
	}
	return (NULL);
}

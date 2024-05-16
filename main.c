/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:12:03 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:35:06 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_program(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->phil_count)
	{
		pthread_mutex_destroy(&program->forks_mutex[i]);
		pthread_mutex_destroy(&program->philosophers[i].full_mutex);
		free(program->philosophers[i].state);
		i++;
	}
	pthread_mutex_destroy(&program->one_philo_has_been_created_mutex);
	pthread_mutex_destroy(&program->all_philos_have_been_created_mutex);
	pthread_mutex_destroy(&program->monitor->printf_mutex);
	pthread_mutex_destroy(&program->monitor->one_philo_is_full_mutex);
	pthread_mutex_destroy(&program->monitor->all_philos_full_mutex);
	pthread_mutex_destroy(&program->monitor->update_meal_counter_mutex);
	pthread_mutex_destroy(&program->monitor->philo_state_mutex);
	pthread_mutex_destroy(&program->monitor->access_mutex);
	free(program->forks_mutex);
	free(program->monitor);
	free(program->philosophers);
	free(program);
}

int	main(int argc, char **argv)
{
	t_program	*program;

	if (verif_args(argc, argv) == 1)
	{
		program = set_dinner_data(argv);
		handle_threads(program);
		clean_program(program);
	}
	return (0);
}

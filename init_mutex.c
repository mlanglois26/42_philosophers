/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:07:08 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:38:19 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_forks(t_program *program)
{
	int	i;
	int	res;

	i = 0;
	while (i < program->phil_count)
	{
		res = pthread_mutex_init(&program->forks_mutex[i], NULL);
		i++;
	}
	i = 0;
	while (i < program->phil_count)
	{
		if (res != 0)
			printf("error en creant fork mutex %d\n", i);
		i++;
	}
}

void	init_program_mutex(t_program *program)
{
	int	res1;
	int	res2;

	init_forks(program);
	res1 = pthread_mutex_init(&program->one_philo_has_been_created_mutex, NULL);
	if (res1 != 0)
		printf("error one philo has been created mutex failed\n");
	res2 = pthread_mutex_init(&program->all_philos_have_been_created_mutex,
			NULL);
	if (res2 != 0)
		printf("error one philo has been created mutex failed\n");
}

void	init_monitor_mutex(t_program *program)
{
	pthread_mutex_init(&program->monitor->one_philo_is_full_mutex, NULL);
	pthread_mutex_init(&program->monitor->all_philos_full_mutex, NULL);
	pthread_mutex_init(&program->monitor->update_meal_counter_mutex, NULL);
	pthread_mutex_init(&program->monitor->philo_state_mutex, NULL);
	pthread_mutex_init(&program->monitor->access_mutex, NULL);
	pthread_mutex_init(&program->monitor->death_flag_mutex, NULL);
	pthread_mutex_init(&program->monitor->printf_mutex, NULL);
}

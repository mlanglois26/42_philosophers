/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:19:31 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:05:46 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	create_monitor(t_program *program)
{
	int	ret;

	ret = pthread_create(&program->monitor->thread_monitor, NULL,
			monitor_routine, (void *)program);
	if (ret != 0)
	{
		printf("Erreur lors de la creation des threads\n");
		exit(0);
	}
}

static void	create_philos(t_program *program)
{
	int	i;
	int	ret;

	i = 0;
	while (i < program->phil_count)
	{
		ret = pthread_create(&program->philosophers[i].thread_id, NULL,
				thread_routine, (void *)&program->philosophers[i]);
		if (ret != 0)
		{
			printf("Erreur lors de la creation des threads\n");
			exit(0);
		}
		pthread_mutex_lock(&program->one_philo_has_been_created_mutex);
		program->philosophers[i].has_been_created = 1;
		pthread_mutex_unlock(&program->one_philo_has_been_created_mutex);
		i++;
	}
}

static void	wait_for_everybody_and_set_starting_time(t_program *program)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	while (nb_of_philo_created(program) != program->phil_count)
		precise_usleep(1000);
	program->all_philo_created = 1;
	program->start_of_program = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
	pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
}

static void	join_threads(t_program *program)
{
	int	i;
	int	ret;

	i = 0;
	while (i < program->phil_count)
	{
		ret = pthread_join(program->philosophers[i].thread_id, NULL);
		if (ret != 0)
		{
			printf("Erreur lors de la jonction des threads\n");
			exit(0);
		}
		i++;
	}
}

void	handle_threads(t_program *program)
{
	create_monitor(program);
	create_philos(program);
	wait_for_everybody_and_set_starting_time(program);
	join_threads(program);
	pthread_join(program->monitor->thread_monitor, NULL);
}

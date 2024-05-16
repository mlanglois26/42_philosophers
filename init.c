/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:42:24 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/16 09:03:36 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo_struct(t_program *program)
{
	int	i;
	int	res;

	i = 0;
	while (i < program->phil_count)
	{
		program->philosophers[i].id = i;
		program->philosophers[i].ptr = program;
		program->philosophers[i].has_been_created = 0;
		program->philosophers[i].meal_counter = 0;
		program->philosophers[i].last_meal_time = -1;
		program->philosophers[i].full = 0;
		program->philosophers[i].is_dead = 0;
		program->philosophers[i].state = malloc(sizeof(int));
		*(program->philosophers[i].state) = THINKING;
		res = pthread_mutex_init(&program->philosophers[i].full_mutex, NULL);
		if (res != 0)
			printf("error full_mutex has been created failed\n");
		i++;
	}
}

t_program	*malloc_program(char **argv)
{
	t_program	*program;

	program = malloc(sizeof(t_program));
	if (!program)
		return (NULL);
	program->monitor = malloc(sizeof(t_monitor));
	if (!program->monitor)
		return (free(program), NULL);
	program->phil_count = ft_atol(argv[1]);
	program->philosophers = malloc(sizeof(t_philo) * program->phil_count);
	if (!program->philosophers)
	{
		free(program->monitor);
		free(program);
		return (NULL);
	}
	program->forks_mutex = malloc(sizeof(pthread_mutex_t)
			* program->phil_count);
	if (!program->forks_mutex)
	{
		free(program->philosophers);
		free(program->monitor);
		return (free(program), NULL);
	}
	return (program);
}

static void	init_program(t_program *program, char **argv)
{
	program->all_philo_created = 0;
	program->all_full = 0;
	program->start_of_program = 0;
	program->one_philo_died = 0;
	program->time_to_die = ft_atol(argv[2]);
	program->time_to_eat = ft_atol(argv[3]);
	program->time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		program->nb_of_time_to_eat = ft_atol(argv[5]);
	else
		program->nb_of_time_to_eat = -1;
}

t_program	*set_dinner_data(char **argv)
{
	t_program	*program;

	program = malloc_program(argv);
	if (!program)
		return (NULL);
	init_program(program, argv);
	init_program_mutex(program);
	init_philo_struct(program);
	init_monitor_mutex(program);
	return (program);
}

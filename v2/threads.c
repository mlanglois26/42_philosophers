/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:19:31 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/13 16:53:33 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
        // printf("------------------------philo has been created = %d\n", program->philosophers[i].has_been_created);
		pthread_mutex_unlock(&program->one_philo_has_been_created_mutex);
		i++;
	}
}

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

// dans create_threads lorsqu un philo est cree son status passe a 1, ici je
// recupere le count des threads crees

static int	nb_of_philo_created(t_program *program)
{
	int	count;
	int	i;

	pthread_mutex_lock(&program->one_philo_has_been_created_mutex);
	count = 0;
	i = 0;
	while (i < program->phil_count)
	{
		if (program->philosophers[i].has_been_created == 1)
			count++;
		i++;
	}
        printf("ciount = %d\n", count);

	pthread_mutex_unlock(&program->one_philo_has_been_created_mutex);
	return (count);
}

static void	wait_for_everybody_and_set_starting_time(t_program *program)
{
	struct timeval	tv;
    // printf("oooooooooooooooooooooooo\n");
	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	while (nb_of_philo_created(program) != program->phil_count)
    {
        // printf("bloquee????\n");
        precise_usleep(1000);
    }
	program->all_philo_created = 1;
	printf("all philo have been created\n");
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
    // pthread_mutex_lock(&program->shared_data_mutex);
    pthread_join(program->monitor->thread_monitor, NULL);
    // pthread_mutex_unlock(&program->shared_data_mutex);

	
	
}

// void	handle_threads(t_program *program)
// {
//     create_monitor(program);
//     create_philos(program);
//     pthread_mutex_lock(&program->shared_data_mutex);
// 	wait_for_everybody_and_set_starting_time(program);
//     pthread_mutex_unlock(&program->shared_data_mutex);
// 	join_threads(program);
// }

void handle_threads(t_program *program)
{
  
    create_philos(program);
    wait_for_everybody_and_set_starting_time(program);

    // pthread_mutex_lock(&program->shared_data_mutex); // Verrouillage des données partagées
    create_monitor(program);
    // pthread_mutex_unlock(&program->shared_data_mutex); // Verrouillage des données partagées

    // while (program->end_of_program_flag == 0)
    //     ;
    // pthread_mutex_lock(&program->shared_data_mutex); // Re-verrouillage des données partagées

    join_threads(program);

    // pthread_mutex_unlock(&program->shared_data_mutex); // Déverrouillage des données partagées
}

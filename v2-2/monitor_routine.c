/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:11:16 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:40:07 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *data)
{
	t_program	*program;
	int			i;

	program = (t_program *)data;
	wait_for_all(program);
	while (death_alert(program) != 1)
	{
		i = 0;
		while (i < program->phil_count && program->end_of_program_flag == 0)
		{
			if (philo_is_dead(&program->philosophers[i]) == 1)
			{
				pthread_mutex_lock(&program->monitor->philo_state_mutex);
				*(program->philosophers[i].state) = DEAD;
				pthread_mutex_unlock(&program->monitor->philo_state_mutex);
				pthread_mutex_lock(&program->monitor->access_mutex);
				write_monitor(program, &program->philosophers[i]);
				pthread_mutex_unlock(&program->monitor->access_mutex);
				// pthread_mutex_lock(&program->monitor->death_flag_mutex);
				// program->end_of_program_flag = 1;
				// pthread_mutex_unlock(&program->monitor->death_flag_mutex);
			}
			i++;
		}
		if (everybody_is_full(program) == 1)
			break ;
	}
	return (NULL);
}


// je pense que le code commente peut degage
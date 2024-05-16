/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:11:16 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/16 09:08:01 by malanglo         ###   ########.fr       */
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
		while (i < program->phil_count && death_alert(program) != 1)
		{
			if (philo_is_dead(&program->philosophers[i]) == 1)
			{
				set_philo_state(program, &program->philosophers[i], DEAD);
				safe_write_monitor(program, &program->philosophers[i]);
			}
			i++;
		}
		if (everybody_is_full(program) == 1)
			break ;
	}
	return (NULL);
}

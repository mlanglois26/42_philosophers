/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 09:56:06 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 09:57:42 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// dans create_threads lorsqu un philo est cree son status passe a 1, ici je
// recupere le count des threads crees

int	nb_of_philo_created(t_program *program)
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
	pthread_mutex_unlock(&program->one_philo_has_been_created_mutex);
	return (count);
}

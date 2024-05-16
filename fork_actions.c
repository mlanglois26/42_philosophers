/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_actions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 15:46:32 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 18:23:20 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_right_fork(t_program *program, t_philo *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;
	pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
	set_philo_state(program, philo, HAS_TAKEN_A_FORK);
	safe_write_monitor(program, philo);
}

static void	take_left_fork(t_program *program, t_philo *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;
	pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
	set_philo_state(program, philo, HAS_TAKEN_A_FORK);
	safe_write_monitor(program, philo);
}

void	pickup_forks(t_program *program, t_philo *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;
	if (death_alert(program) == 1)
		return ;
	if ((philo->id + 1) - program->phil_count == 0)
	{
		take_right_fork(program, philo);
		take_left_fork(program, philo);
	}
	else
	{
		take_left_fork(program, philo);
		take_right_fork(program, philo);
	}
}

void	putdown_forks(t_program *program, t_philo *philo)
{
	struct timeval	tv;
	int				left_fork_id;
	int				right_fork_id;

	gettimeofday(&tv, NULL);
	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % program->phil_count;
	if ((philo->id + 1) - program->phil_count == 0)
	{
		pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
		pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
	}
	else
	{
		pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
		pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
	}
	if (death_alert(program) == 1)
		return ;
	if (everybody_is_full(program) == 1)
		return ;
	set_philo_state(program, philo, SLEEPING);
	safe_write_monitor(program, philo);
	precise_usleep(get_milli(program->time_to_sleep));
}

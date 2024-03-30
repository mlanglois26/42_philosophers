/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 10:13:53 by malanglo          #+#    #+#             */
/*   Updated: 2024/03/29 16:41:54 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>


# define THINKING 0
# define EATING 1
# define SLEEPING 2
# define DEAD 3

typedef struct s_philo
{
	int				id;
	pthread_t 		thread_id;
	double			life_starting_time;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meal_count;
	int				*state;
    void            *ptr; // pointe vers le program dont fait parti le philo
    long            *last_meal_time; // activer ca lorsque mon philo pose ses forks
	long            *blocktime; // total tps du un philo bloque - ne peut pas etre > que time-to-die - time-to-eat
}					t_philo;

typedef struct s_program
{
	t_philo			*philosophers;
	int				phil_count;
    int 			end_of_program;
	long 			start_of_program;
	pthread_mutex_t	*forks_mutex;
	pthread_t		*script;
    pthread_t 		*monitor;
}					t_program;

/* init */

void init_forks(t_program *program);
void init_philosophers(t_program *program, int nb_of_philo, long time_to_die, long time_to_eat, long time_to_sleep);
void init_program(t_program *program, struct timeval *tv);

/* actions */

void pickup_forks(t_program *program, t_philo *philo);

/* time */

void precise_usleep(long time_to_do_smthg);


void fill_data_input(t_philo *philo, char **argv);

#endif
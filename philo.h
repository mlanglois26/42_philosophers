/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:23:00 by malanglo          #+#    #+#             */
/*   Updated: 2024/04/26 15:29:49 by malanglo         ###   ########.fr       */
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
# define HAS_TAKEN_A_FORK 2
# define SLEEPING 3
# define DEAD 4

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nb_of_time_to_eat;
	int				meal_counter;
	int				has_been_created;
	int				full;
	long			philo_birth;
	int				*state;
	void			*ptr;
	pthread_mutex_t	update_meal_counter_mutex;
	pthread_mutex_t	full_mutex;
	long			last_meal_time;
}					t_philo;

typedef struct s_program
{
	t_philo			*philosophers;
	int				phil_count;
	int				all_philo_created;
	int				all_full;
	long			start_of_program;
	pthread_mutex_t	*forks_mutex;
	pthread_mutex_t	one_philo_has_been_created_mutex;
	pthread_mutex_t	all_philos_have_been_created_mutex;
	pthread_mutex_t	nb_of_full_philo_mutex;
	pthread_mutex_t	*printf_mutex;
	pthread_mutex_t	all_full_mutex;
	pthread_mutex_t	check_for_death_mutex;
	pthread_mutex_t stop_mutex;
}					t_program;

/* utils.c */

int					ft_strlen(char *str);
int					is_only_pos_digit(char *str);
long				ft_atol(char *nptr);

/* parsing.c */

int					verif_args(int argc, char **argv);

/* init.c */

t_program			*init_program(char **argv);
t_program			*handle_program_mutexes(t_program *program);

/* threads.c */

void				handle_threads(t_program *program);

/* routine_utils.c */

void				wait_for_all(t_program *program);
int					nb_of_full_philos(t_program *program);
int					philo_is_dead(t_philo *philo);
int					end_of_program(t_program *program);
int					all_full(t_program *program);

/* routine.c */

void				*thread_routine(void *data);
void				write_monitor(t_program *program, t_philo *philo);

/* time.c */

long				get_milli(int nb);
void				precise_usleep(long time_to_do_smthg);

/* actions */

void				pickup_forks(t_program *program, t_philo *philo);
void				putdown_forks(t_program *program, t_philo *philo);
int					can_i_eat(t_program *program, t_philo *philo);

#endif

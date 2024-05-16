/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:37:23 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/16 09:03:32 by malanglo         ###   ########.fr       */
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
	int				meal_counter;
	int				has_been_created;
	long			last_meal_time;
	int				full;
	int				is_dead;
	int				*state;
	void			*ptr;
	pthread_mutex_t	full_mutex;
}					t_philo;

typedef struct s_monitor
{
	pthread_t		thread_monitor;
	pthread_mutex_t	update_meal_counter_mutex;
	pthread_mutex_t	one_philo_is_full_mutex;
	pthread_mutex_t	all_philos_full_mutex;
	pthread_mutex_t	check_for_death_mutex;
	pthread_mutex_t	access_mutex;
	pthread_mutex_t	philo_state_mutex;
	pthread_mutex_t	death_flag_mutex;
	pthread_mutex_t	printf_mutex;

}					t_monitor;

typedef struct s_program
{
	t_philo			*philosophers;
	t_monitor		*monitor;
	int				phil_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nb_of_time_to_eat;
	int				all_philo_created;
	int				all_full;
	long			start_of_program;
	int				one_philo_died;
	pthread_mutex_t	*forks_mutex;
	pthread_mutex_t	one_philo_has_been_created_mutex;
	pthread_mutex_t	all_philos_have_been_created_mutex;
}					t_program;

/* parsing_utils.c */

int					ft_strlen(char *str);
int					is_only_pos_digit(char *str);
long				ft_atol(char *nptr);

/* parsing.c */

int					verif_args(int argc, char **argv);

/* init_mutex.c */

void				init_forks(t_program *program);
void				init_program_mutex(t_program *program);
void				init_monitor_mutex(t_program *program);

/* init.c */

t_program			*set_dinner_data(char **argv);

/* thread_utils.c */

int					nb_of_philo_created(t_program *program);

/* threads.c */

void				handle_threads(t_program *program);

/* time.c */

long				get_milli(int nb);
void				precise_usleep(long time_to_do_smthg);

/* helper_ph_routine.c */

void				eat(t_program *program, t_philo *philo);
void				think_accordingly(t_program *program, t_philo *philo);
void				set_philo_state(t_program *program, t_philo *philo,
						int assign_state);

/* philo_routine.c */

void				wait_for_all(t_program *program);
void				*thread_routine(void *data);

/* fork_actions.c */

void				pickup_forks(t_program *program, t_philo *philo);
void				putdown_forks(t_program *program, t_philo *philo);

/* monitor_utils_death.c */

int					philo_is_dead(t_philo *philo);
int					death_alert(t_program *program);
int					get_death_flag_value(t_program *program);

/* monitor_utils_full.c */

void				update_meal_counter(t_philo *philo);
void				check_if_full(t_philo *philo);
int					nb_of_full_philos(t_program *program);
int					everybody_is_full(t_program *program);

/* write_actions.c */

void				safe_write_monitor(t_program *program, t_philo *philo);
void				write_monitor(t_program *program, t_philo *philo);

/* monitor_routine.c */

void				*monitor_routine(void *data);

#endif
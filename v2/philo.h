/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:37:23 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/13 18:09:07 by malanglo         ###   ########.fr       */
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
    pthread_mutex_t	new_update_meal_counter_mutex;
    pthread_mutex_t	one_philo_is_full_mutex;
	pthread_mutex_t	all_philos_full_mutex;
    pthread_mutex_t	check_for_death_mutex;
    // pthread_mutex_t	printf_mutex;
    
}   t_monitor;

typedef struct s_program
{
	t_philo			*philosophers;
    t_monitor       *monitor;
	int				phil_count;
    long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nb_of_time_to_eat;
	int				all_philo_created;
	int				all_full;
	long			start_of_program;
	int             end_of_program_flag;
	pthread_mutex_t	*forks_mutex;
    pthread_mutex_t	one_philo_has_been_created_mutex;
	pthread_mutex_t	all_philos_have_been_created_mutex;
    pthread_mutex_t shared_data_mutex;
    pthread_mutex_t	printf_mutex;
    pthread_mutex_t access_mutex;
}					t_program;



/* utils.c */

int					ft_strlen(char *str);
int					is_only_pos_digit(char *str);
long				ft_atol(char *nptr);

/* parsing.c */

int					verif_args(int argc, char **argv);

/* threads.c */

void				handle_threads(t_program *program);

/* time.c */

long				get_milli(int nb);
void				precise_usleep(long time_to_do_smthg);

/* routine.c */

void				*thread_routine(void *data);
void                *monitor_routine(void *data);

/* init.c */

t_program *set_dinner_data(char **argv);

void	wait_for_all(t_program *program);

/* actions.c */

void	pickup_forks(t_program *program, t_philo *philo);
void	putdown_forks(t_program *program, t_philo *philo);
void write_monitor(t_program *program, t_philo *philo);


// en revenant de pause je check les routines

#endif
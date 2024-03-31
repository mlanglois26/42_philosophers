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
	int				    id;
	pthread_t 		    thread_id;
	long			    time_to_die;
	long			    time_to_eat;
	long			    time_to_sleep;
	int				    meal_count;
    int                 has_been_created;
	int				    *state;
    void                *ptr;
}					t_philo;

typedef struct s_program
{
	t_philo			    *philosophers;
	int				    phil_count;
    int                 all_philo_created;
    int 			    end_of_program;
	long 			    start_of_program;
    pthread_mutex_t     mutex;
	pthread_mutex_t	    *forks_mutex;
	pthread_mutex_t		*printf_mutex;
}					t_program;


int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int is_only_pos_digit(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[0] == '+')
          i++;
        if (!(str[i] >= '0' && str[i] <= '9'))
          return (0);
        i++;
    }
    return (1);
}

long	ft_atol(char *nptr)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
  if (nptr[0] == '+')
          i++;
	while (nptr[i])
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
  if (res > 2147483647)
  {
      printf("Error: INT MAX is the limit\n");
      return (0);
  }
	return (res);
}

int is_valid_1_6(char *argv)
{
    int nb;

    if (is_only_pos_digit(argv) == 0)
    {
        printf("Error: Argv must be a positive digit > 0\n");
        return (0);
    }
    if (ft_strlen(argv) > 10)
    {
        printf("Error: INT MAX is the limit\n");
        return (0);
    }
    else
        nb = ft_atol(argv);
    if (nb > 0)
        return (1);
    else
        return (0);
}

int is_valid_2_3_4(char *argv)
{
    int nb;

    if ((is_only_pos_digit(argv) == 0))
    {
        printf("Eroor: Argv 2 | 3 | 4 must be positive digits > 60 ms\n");
        return (0);
    }
    if (ft_strlen(argv) > 10)
    {
        printf("Error: INT MAX is the limit\n");
        return (0);
    }
    else
        nb = ft_atol(argv);
    if (nb < 60)
    {
        printf("Error: Argv 2 | 3 | 4 must be positive digits > 60 ms\n");
        return (0);
    }
    return (1);
}


int verif_args(int argc, char **argv)
{
    if (argc != 5 && argc != 6)
    {
      printf("Usage: ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] and if wanted [nb_of_times_each_philosopher_must_eat]\n");
      return (0);
    }
    if (is_valid_1_6(argv[1]) == 0)
      return (0);
    if (is_valid_2_3_4(argv[2]) == 0)
      return (0);
    if (is_valid_2_3_4(argv[3]) == 0)
      return (0);
    if (is_valid_2_3_4(argv[4]) == 0)
      return (0);
    if (argc == 6)
    {
        if (is_valid_1_6(argv[5]) == 0)
            return (0);
    }
    return (1);
}

void init_forks(t_program *program)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        pthread_mutex_init(&program->forks_mutex[i], NULL);
        i++; 
    }
}

t_philo *init_philo_malloc(t_program *program)
{
    t_philo *philo;
    
    philo = malloc(sizeof(t_philo));
    if (!philo)
        return (NULL);
     philo->state = malloc(sizeof(int));
    if (!philo->state)
    {
        free (philo);
        return (NULL);
    }
    return (philo);
}

t_program *init_program(int nb1)
{   
    t_program *program;
    
    program = malloc(sizeof(t_program));
    if (!program)
        return (NULL);
    program->all_philo_created = 0;
    program->phil_count = nb1;
    program->philosophers = malloc(sizeof(t_philo) * program->phil_count);
    if (!program->philosophers)
    {
        free(program);
        return (NULL);
    }
    program->forks_mutex = malloc(sizeof(pthread_mutex_t) * program->phil_count);
    if (!program->forks_mutex)
    {
        free(program->philosophers);
        free(program);
        return (NULL);
    }
    init_forks(program);
    program->printf_mutex = malloc(sizeof(pthread_mutex_t));
    if (!program->printf_mutex)
    {
        free(program->philosophers);
        free(program->forks_mutex);
        free(program);
        return (NULL);
    }
    pthread_mutex_init(program->printf_mutex, NULL);
    return (program);
}

void fill_philo_struct(t_program *program, char **argv)
{
    int i;
    // struct timeval tv;

    // gettimeofday(&tv, NULL);
    i = 0;
    while (i < program->phil_count)
    {
        program->philosophers[i] = *init_philo_malloc(program);
        program->philosophers[i].id = i + 1;
        program->philosophers[i].ptr = program;
        program->philosophers[i].has_been_created = 0;
        program->philosophers[i].time_to_die = ft_atol(argv[2]);
        program->philosophers[i].time_to_eat = ft_atol(argv[3]);
        program->philosophers[i].time_to_sleep = ft_atol(argv[4]);
        if (argv[5])
            program->philosophers[i].meal_count = ft_atol(argv[5]);
        else
            program->philosophers[i].meal_count = -1;
        i++;
    }
}

void print_philo_struct(t_philo *philo) 
{
    printf("philo id: %d\n", philo->id);
    printf("time to die: %ld\n", philo->time_to_die);
    printf("time to eat: %ld\n", philo->time_to_eat);
    printf("time to sleep: %ld\n", philo->time_to_sleep);
    printf("meal_count: %d\n", philo->meal_count);

}

void print_program_struct(t_program *program) 
{
    printf("Phil_count: %d\n", program->phil_count);
}

void script_monitor(t_program *program, t_philo *philo)
{
    int state;
    struct timeval tv;
    long current_time;

    gettimeofday(&tv, NULL);

    pthread_mutex_lock(program->printf_mutex);
    
    state = (*(philo->state));
    current_time = (tv.tv_sec * 1e3 + tv.tv_usec / 1e3) - program->start_of_program;

   
    // printf("dans printf monitor -> program start = %ld\n", program->start_of_program);
    // printf("current time = %f\n", (tv.tv_sec * 1e3 + tv.tv_usec / 1e3));

    if (state == THINKING)
        printf("%ld %d is thinking\n", current_time, philo->id); // putain c est des microsecondsssssssssssssss === je veux des milllllli
    else if (state == EATING)
        printf("%ld %d is eating\n", current_time, philo->id);
    else if (state == SLEEPING)
        printf("%ld %d is sleeping\n", current_time, philo->id);
    else if (state == DEAD)
        printf("%ld %d is dead\n", current_time, philo->id);
    else if (state == HAS_TAKEN_A_FORK)
        printf("%ld %d has taken a fork\n", current_time, philo->id);
    else
        printf("Dinner is over\n");

    pthread_mutex_unlock(program->printf_mutex);

}

int nb_of_philo_ready(t_program *program)
{
    int count;
    int i;

    count = 0;
    i = 0;
    while (i < program->phil_count)
    {
        if (program->philosophers[i].has_been_created == 1)
            count++;
        i++;
    }
    return (count);
}

void *thread_routine(void *data)
{
    t_philo *philo;
    t_program *program;

    philo = (t_philo *)data;
    program = (t_program *)philo->ptr;;
    
    int left_fork_id;
    int right_fork_id;

    left_fork_id = philo->id;
    right_fork_id = (philo->id + 1) % program->phil_count;

    // wait for monitor signal to say start routine
    // monitor_approval(program);
    // while (nb_of_philo_ready(program) == 1)
    //     usleep(10000);

    while (program->all_philo_created != 1)
        usleep(10000);

    if (program->all_philo_created == 1)
    {
        int i = 0;
        while (i < 2)
        {
            // // pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
            // *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
            // script_monitor(program, philo);
            
            // // pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
            // // *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
            // // script_monitor(program, philo);
            
            // // *(program->philosophers[philo->id].state) = EATING;
            // // script_monitor(program, philo);
            // // usleep(philo->time_to_eat);

            // // pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
            // // pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
            *(philo->state) = SLEEPING;
            script_monitor(program, philo);
            usleep(philo->time_to_sleep);

            i++;
        }
    }
    else
        printf("philo are not all ready\n");

    return (NULL);
}

void create_philo_threads(t_program *program)
{
    int i;
    int ret;
    struct timeval tv;

    pthread_mutex_init(&program->mutex, NULL);

    i = 0;
    while (i < program->phil_count)
    {
        ret = pthread_create(&program->philosophers[i].thread_id, NULL, thread_routine, (void *)&program->philosophers[i]);
        if (ret != 0)
		{
			printf("Erreur lors de la creation des threads\n");
			exit(0);
		}
        program->philosophers[i].has_been_created = 1;
        i++;
    }

    pthread_mutex_lock(&program->mutex);
    while (nb_of_philo_ready(program) != program->phil_count) 
    {
        pthread_mutex_unlock(&program->mutex);
        usleep(10000);
        pthread_mutex_lock(&program->mutex);
    }
    pthread_mutex_unlock(&program->mutex);

    program->all_philo_created = 1;

    gettimeofday(&tv, NULL);
    program->start_of_program = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    printf("dans create philo -> program start = %ld\n", program->start_of_program);

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
}


// void create_philo_threads(t_program *program)
// {
//     int i;
//     int ret;
//     struct timeval tv;

//     i = 0;
//     while (i < program->phil_count)
//     {
//         ret = pthread_create(&program->philosophers[i].thread_id, NULL, thread_routine, (void *)&program->philosophers[i]);
//         if (ret != 0)
// 		{
// 			printf("Erreur lors de la creation des threads\n");
// 			exit(0);
// 		}
//         program->philosophers[i].has_been_created = 1;
//         i++;
//     }

//     gettimeofday(&tv, NULL);
//     program->start_of_program = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
//     printf("dans create philo -> program start = %ld\n", program->start_of_program);

//     while (nb_of_philo_ready(program) == 1)
//         usleep(10000);
//     program->all_philo_created = 1;

//     i = 0;
//     while (i < program->phil_count)
// 	{
// 		ret = pthread_join(program->philosophers[i].thread_id, NULL);
// 		if (ret != 0)
// 		{
// 			printf("Erreur lors de la jonction des threads\n");
// 			exit(0);
// 		}
// 		i++;
// 	}
// }

int	main(int argc, char **argv)
{
    t_program *program;
  
    if (verif_args(argc, argv) == 1)
        printf("all good with args\n");
    else
    {
        printf("pb with args\n");
        exit (0);
    }
    
    program = init_program(ft_atol(argv[1]));
    printf("phil count = %d\n", program->phil_count);
    
    fill_philo_struct(program, argv);

    print_program_struct(program);
    
    int i = 0; 
    while (i < program->phil_count) 
    {
        printf("\nPhilosopher %d:\n", i + 1);
        print_philo_struct(&program->philosophers[i]);
        i++;
    }
    printf("\n\n");
    create_philo_threads(program);
	return (0);
}


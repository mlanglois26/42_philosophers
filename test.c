
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
	int				    id;
	pthread_t 		    thread_id;
	long			    time_to_die;
	long			    time_to_eat;
	long			    time_to_sleep;
	int				    meal_count;
	int				    *state;
    void                *ptr; 
    long                *last_meal_time; 
    long                *blocktime; 
}					t_philo;

typedef struct s_program
{
	t_philo			    *philosophers;
	int				    phil_count;
    int 			    end_of_program;
	long 			    start_of_program;
	pthread_mutex_t	*   forks_mutex;
	pthread_t		    *script;
    pthread_t 		    *monitor;
    pthread_t           *clock_mutex;
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

/* init */

// void init_program(t_program *program)
// {   
//     program->philosophers = malloc(sizeof(t_philo) * program->phil_count);
//     program->forks_mutex = malloc(sizeof(pthread_mutex_t) * program->phil_count);
//     init_forks(program);
// }


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
    philo->last_meal_time = malloc(sizeof(long));
    if (!philo->last_meal_time)
    {
        free(philo);
        return (NULL);
    }
    philo->blocktime = malloc(sizeof(long));
    if (!philo->blocktime)
    {
        free(philo->last_meal_time);
        free(philo);
        return (NULL);
    }
    philo->state = malloc(sizeof(int));
    if (!philo->state)
    {
        free(philo->last_meal_time);
        free(philo->blocktime);
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
    pthread_mutex_init(&program->clock_mutex, NULL); // maybe wrong
    return (program);
}


void fill_philo_struct(t_program *program, char **argv)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        program->philosophers[i] = *init_philo_malloc(program);
        program->philosophers[i].id = i + 1;
        program->philosophers[i].ptr = program;
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




void *thread_routine(void *data)
{
    t_philo *philo;
    t_program *program;

    philo = (t_philo *)data;
    program = (t_program *)philo->ptr;;
   
    // mettre une mutex ?
    while (philo->life_starting_time != program->start_of_program)
        ;
    
    return (NULL);
}




void create_philo_threads(t_program *program)
{
    int i;
    int ret;
    struct timeval tv;

    i = 0;
    while (i < program->phil_count)
    {
        ret = pthread_create(&program->philosophers[i].thread_id, NULL, thread_routine, (void *)&program->philosophers[i]);
        if (ret != 0)
		{
			printf("Erreur lors de la creation des threads\n");
			exit(0);
		}
        i++;
    }
    
    gettimeofday(&tv, NULL);
    program->start_of_program = ((tv.sec * 1e3) + (tv.usec / 1e3));
    
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

	return (0);
}




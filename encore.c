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
    int                 nb_of_time_to_eat;
	int				    meal_counter;
    int                 has_been_created;
	int				    *state;
    void                *ptr;
}					t_philo;

typedef struct s_program
{
	t_philo			    *philosophers;
	int				    phil_count;
    int                 all_philo_created;
	long 			    start_of_program;
	pthread_mutex_t	    *forks_mutex;
	pthread_mutex_t		*printf_mutex;
    pthread_mutex_t     one_philo_has_been_created_mutex;
    pthread_mutex_t     all_philos_have_been_created_mutex;
    pthread_mutex_t     update_meal_counter_mutex;
    pthread_mutex_t     nb_of_full_philo_mutex;
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
        printf("Error: Argv 2 | 3 | 4 must be positive digits > 60 ms\n");
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
    int res;

    i = 0;
    while (i < program->phil_count)
    {
        res = pthread_mutex_init(&program->forks_mutex[i], NULL);
        i++; 
    }

    i = 0;
    while (i < program->phil_count)
    {
        if (res != 0)
            printf("error en creant mutex %d\n", i);
        else
            printf("mutex %d a bien ete cree\n", i);
        i++;
    }
}

/* init */

t_program *init_program(int nb1)
{   
    t_program *program;
    
    program = malloc(sizeof(t_program));
    if (!program)
        return (NULL);
    program->phil_count = nb1;
    program->all_philo_created = 0;
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

    int res = pthread_mutex_init(program->printf_mutex, NULL);
    if (res != 0)
        printf("error mutex pour printf failed\n");
    else
        printf("mutex pour printf a bien ete cree\n");

    int res2 = pthread_mutex_init(&program->all_philos_have_been_created_mutex, NULL);
    if (res2 != 0)
        printf("error mutex pour all_philos_have_been_created_mutex_mutex failed\n");
    else
        printf("mutex pour all_philos_have_been_created_mutex_mutex a bien ete cree\n");
    
    int res4 = pthread_mutex_init(&program->one_philo_has_been_created_mutex, NULL);
        if (res4 != 0)
        printf("error one philo has been created mutex failed\n");
    else
        printf("one philo has been created mutex a bien ete cree\n");
    int res5 = pthread_mutex_init(&program->update_meal_counter_mutex, NULL);
        if (res5 != 0)
        printf("error meal_counter_mutex has been created mutex failed\n");
    else
        printf("meal_counter_mutex has been created mutex a bien ete cree\n");
    int res6 = pthread_mutex_init(&program->nb_of_full_philo_mutex, NULL);
        if (res6 != 0)
        printf("error nb_of_full_philo_mutex has been created mutex failed\n");
    else
        printf("nb_of_full_philo_mutex has been created mutex a bien ete cree\n");
    return (program);
}

t_philo *malloc_philo()
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

void fill_philo_struct(t_program *program, char **argv)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        program->philosophers[i] = *malloc_philo();
        if (program->philosophers[i].state == NULL) 
        {
            // La mémoire n'a pas pu être allouée, faites quelque chose, comme quitter le programme ou gérer l'erreur d'une autre manière
            printf("Erreur lors de l'allocation de mémoire pour philosophe %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
        else 
            printf("it worked for philo alloc\n");
       
        program->philosophers[i].id = i + 1;
        // printf("program->philosophers[i] = %d\n", i);
        program->philosophers[i].ptr = program;
        program->philosophers[i].has_been_created = 0;
        program->philosophers[i].meal_counter = 0;
        program->philosophers[i].time_to_die = ft_atol(argv[2]);
        program->philosophers[i].time_to_eat = ft_atol(argv[3]);
        program->philosophers[i].time_to_sleep = ft_atol(argv[4]);
        if (argv[5])
            program->philosophers[i].nb_of_time_to_eat = ft_atol(argv[5]);
        else
            program->philosophers[i].nb_of_time_to_eat = -1;
        i++;
    }
}


  
int     nb_of_philo_created(t_program *program)
{
    int count;
    int i;

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


int pickup_right_fork(t_program *program, t_philo *philo)
{
    int right_fork_id;

    right_fork_id = (philo->id + 1) % program->phil_count;

    pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
    printf("Philo %d has pick UP fork %d\n", philo->id, right_fork_id);
    return (1);
}

int pickup_left_fork(t_program *program, t_philo *philo)
{
    int left_fork_id;

    left_fork_id = philo->id ;

    pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
    printf("Philo %d has pick UP fork %d\n", philo->id, left_fork_id);
    return (1);
}

void putdown_forks(t_program *program, t_philo *philo)
{
    int left_fork_id;
    int right_fork_id;

    left_fork_id = philo->id;
    right_fork_id = (philo->id + 1) % program->phil_count;
    
    pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
    printf("Philo %d has put DOWN fork %d\n", philo->id, left_fork_id);

    pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
    printf("Philo %d has put DOWN fork %d\n", philo->id, right_fork_id);
}

int philo_has_both_forks(t_program *program, t_philo *philo)
{
    if ((pickup_left_fork(program, philo) == 1) && (pickup_right_fork(program, philo) == 1))
        return (1);
    return (0);
}







void update_meal_counter(t_program *program, t_philo *philo) // ca c est good
{
    printf("\n\n entrer dans last meal counter\n");
    printf("avant le lock - philo %d meal counter = %d\n", philo->id, philo->meal_counter);
    pthread_mutex_lock(&program->update_meal_counter_mutex);
    philo->meal_counter++;
    pthread_mutex_unlock(&program->update_meal_counter_mutex);
    printf("apres le lock - philo %d meal counter = %d\n", philo->id, philo->meal_counter);
    printf("exit last meal counter\n\n\n");
}


int nb_of_full_philos(t_program *program)
{
    t_philo *philo;
    int count = 0;
    int i = 0;
    
    printf("\n\nentrer dans nb of full philos\n");
    pthread_mutex_lock(&program->nb_of_full_philo_mutex);
    printf("program->phil_count = %d\n", program->phil_count);
    while (i < program->phil_count)
    {
        philo = &program->philosophers[i];
        printf("nb of time philo %d must eat = %d\n", philo->id, philo->nb_of_time_to_eat);
        printf("%d | philo %d meal counter = %d\n", i, philo->id, philo->meal_counter);
        if (philo->meal_counter >= philo->nb_of_time_to_eat)
            count++;
        i++;
    }
  
    pthread_mutex_unlock(&program->nb_of_full_philo_mutex);
      printf("---------count des philos full = %d--------------------\n", count);
    printf("exit dans nb of full philos\n\n\n");
    return (count);
}

int end_of_program(t_program *program)
{
    printf("\n\nentering end of prgram\n");
    if (nb_of_full_philos(program) == program->phil_count)
        return (1);
    else
        return (0);
}


void *thread_routine(void *data) 
{
    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;

    pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
    while (program->all_philo_created != 1) 
    {
        pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
        printf("Philosophe %d: ATTEND que tous les philosophes soient créés\n", philo->id);
        usleep(100000);
        pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
    }
    pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);

    int left_fork_id;
    int right_fork_id;

    left_fork_id = (philo->id - 1); // fork 0 correspond a philo 0
    // printf("left fork id = %d\n", left_fork_id);
    right_fork_id = philo->id % program->phil_count;
    // printf("right fork id %d\n", right_fork_id);

    if (philo->id % 2 == 0)
        usleep(1000);
   
    while (end_of_program(program) == 0) // || only one philo
    {
        // printf("meal count du philo %d = %d\n", philo->id, program->philosophers[philo->id].meal_counter);
        // if (philo_has_both_forks(program, philo) == 1)
        // {
        //     *(program->philosophers[philo->id].state) = EATING;
        //     printf("Philo %d is eating\n", philo->id);
        //     usleep(philo->time_to_eat);
        //     putdown_forks(program, philo);

        //     update_meal_counter(program, philo);

        //     printf("Philo %d has eaten %d times\n", philo->id, philo->meal_counter);
        //     *(program->philosophers[philo->id].state) = SLEEPING;
        //     printf("Philo %d is sleeping\n", philo->id);
        //     usleep(philo->time_to_sleep);
        //     *(program->philosophers[philo->id].state) = THINKING;
        //     printf("Philo %d is thinking\n", philo->id);
        // } 

        pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
        printf("Philo %d has pick UP fork %d\n", philo->id, left_fork_id);
        pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
        printf("Philo %d has pick UP fork %d\n", philo->id, right_fork_id);
        printf("Philo %d is eating\n", philo->id);
        // trigger clock
        usleep(philo->time_to_eat);
        pthread_mutex_unlock(&program->forks_mutex[left_fork_id]);
        printf("Philo %d has put DOWN fork %d\n", philo->id, left_fork_id);
        pthread_mutex_unlock(&program->forks_mutex[right_fork_id]);
        printf("Philo %d has put DOWN fork %d\n", philo->id, right_fork_id);
        update_meal_counter(program, philo);
        printf("Philo %d is sleeping\n", philo->id);
        usleep(philo->time_to_sleep);
        printf("Philo %d is thinking\n", philo->id);
    }
    return NULL;
}


void create_philo_threads(t_program *program)
{
    int i;
    int ret;


    i = 0;
    while (i < program->phil_count)
    {
        ret = pthread_create(&program->philosophers[i].thread_id, NULL, thread_routine, (void *)&program->philosophers[i]);
        if (ret != 0)
		{
			printf("Erreur lors de la creation des threads\n");
			exit(0);
		}
        pthread_mutex_lock(&program->one_philo_has_been_created_mutex); 
        program->philosophers[i].has_been_created = 1;
        pthread_mutex_unlock(&program->one_philo_has_been_created_mutex); 
        printf("Philosophe %d créé\n", i);
        i++;
    }

    pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
    while (nb_of_philo_created(program) != program->phil_count)
        usleep(1000);
    pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);

    pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
    
    program->all_philo_created = 1;
    printf("all philos have been created - updating program->all_philo_created = 1\n");
    struct timeval	tv;
    gettimeofday(&tv, NULL);
    program->start_of_program = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
    printf("-> program start = %ld\n", program->start_of_program);
    
    pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);

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






void print_philo_struct(t_philo *philo) 
{
    printf("philo id: %d\n", philo->id);
    printf("time to die: %ld\n", philo->time_to_die);
    printf("time to eat: %ld\n", philo->time_to_eat);
    printf("time to sleep: %ld\n", philo->time_to_sleep);
    printf("nb_of_time_to_eat: %d\n", philo->nb_of_time_to_eat);
    printf("philo has been created = %d\n", philo->has_been_created);
}

void print_program_struct(t_program *program) 
{
    printf("Phil_count: %d\n", program->phil_count);
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




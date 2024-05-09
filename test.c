#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define THINKING 0
#define EATING 1
#define HAS_TAKEN_A_FORK 2
#define SLEEPING 3
#define DEAD 4

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
	int				is_dead;
	long			philo_birth;
	int				*state;
	void			*ptr;
	pthread_mutex_t	full_mutex;
    int             must_stop;
	long			last_meal_time;
}					t_philo;

typedef struct s_program
{
	t_philo			*philosophers;
	int				phil_count;
	int				all_philo_created;
	int				all_full;
	long			start_of_program;
	pthread_t		monitor;
	pthread_mutex_t	*forks_mutex;
	pthread_mutex_t	one_philo_has_been_created_mutex;
	pthread_mutex_t	all_philos_have_been_created_mutex;
	pthread_mutex_t	one_philo_is_full_mutex;
	pthread_mutex_t	all_philo_full_mutex;
	pthread_mutex_t	printf_mutex;
	pthread_mutex_t	all_full_mutex;
	pthread_mutex_t	check_for_death_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	test;
	pthread_mutex_t	global_mutex;
	pthread_mutex_t	clock_mutex;
	int				end_of_program_flag;
	pthread_mutex_t	protection_mutex;
	pthread_mutex_t	other;
	pthread_mutex_t	new_update_meal_counter_mutex;
	pthread_mutex_t	philo_state_mutex;
	pthread_mutex_t	access_mutex;
    pthread_mutex_t	again_mutex;
}					t_program;


int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	is_only_pos_digit(char *str)
{
	int	i;

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


static int	is_valid_1(char *argv)
{
	int	nb;

	if (is_only_pos_digit(argv) == 0)
	{
		printf("Error: Argv 1 must be a positive digit > 0\n");
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

static int	is_valid_6(char *argv)
{
	int	nb;

	if (is_only_pos_digit(argv) == 0)
	{
		printf("Error: Argv 6 must be a positive digit >= 0\n");
		return (0);
	}
	if (ft_strlen(argv) > 10)
	{
		printf("Error: INT MAX is the limit\n");
		return (0);
	}
	else
		nb = ft_atol(argv);
	if (nb >= 0)
		return (1);
	else
		return (0);
}

static int	is_valid_2_3_4(char *argv)
{
	int	nb;

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

int	verif_args(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo [number_of_philosophers] [time_to_die]\
        [time_to_eat] [time_to_sleep] and if wanted\
        [nb_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	if (is_valid_1(argv[1]) == 0)
		return (0);
	if (is_valid_2_3_4(argv[2]) == 0)
		return (0);
	if (is_valid_2_3_4(argv[3]) == 0)
		return (0);
	if (is_valid_2_3_4(argv[4]) == 0)
		return (0);
	if (argc == 6)
	{
		if (is_valid_6(argv[5]) == 0)
			return (0);
	}
	return (1);
}


void handle_philo_mutexes(t_program *program, int i)
{
    int res_2;
    
    res_2 = pthread_mutex_init(&program->philosophers[i].full_mutex, NULL);
    if (res_2 != 0)
        printf("error full_mutex has been created mutex failed\n");
}

void fill_philo_struct(t_program *program, char **argv)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        program->philosophers[i].id = i;
        program->philosophers[i].ptr = program;
        program->philosophers[i].has_been_created = 0;
        program->philosophers[i].meal_counter = 0;
        program->philosophers[i].last_meal_time = -1;
        program->philosophers[i].philo_birth = -1;
        program->philosophers[i].full = 0;
         program->philosophers[i].must_stop = 0;
        program->philosophers[i].is_dead = 0;
        program->philosophers[i].state = malloc(sizeof(int));
        program->philosophers[i].time_to_die = ft_atol(argv[2]);
        program->philosophers[i].time_to_eat = ft_atol(argv[3]);
        program->philosophers[i].time_to_sleep = ft_atol(argv[4]);
        if (argv[5])
            program->philosophers[i].nb_of_time_to_eat = ft_atol(argv[5]);
        else
            program->philosophers[i].nb_of_time_to_eat = -1;
        handle_philo_mutexes(program, i);
        i++;
    }
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
            printf("error en creant fork mutex %d\n", i);
        i++;
    }
}

t_program *handle_program_mutexes(t_program *program)
{

   int res11 = pthread_mutex_init(&program->new_update_meal_counter_mutex, NULL);
    if (res11 != 0)
        printf("error mutex pour update_meal_counter_mutex_mutex failed\n");
    else
        printf("mutex pour all_philos_have_been_created_mutex_mutex a bien ete cree\n");
    
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

    int res22 = pthread_mutex_init(&program->one_philo_is_full_mutex, NULL);
    if (res22 != 0)
        printf("error mutex pour onephiloisfuul_mutex failed\n");
    else
        printf("mutex pour onephiloisfuul_mutex a bien ete cree\n");
    
    int res24 = pthread_mutex_init(&program->all_philo_full_mutex, NULL);
        if (res24 != 0)
        printf("error one philo has been created mutex failed\n");
    else
        printf("one philo has been created mutex a bien ete cree\n");


    int res = pthread_mutex_init(&program->printf_mutex, NULL);
    if (res != 0)
        printf("error mutex pour printf failed\n");
    else
        printf("mutex pour printf a bien ete cree\n");
    

    int res9 = pthread_mutex_init(&program->check_for_death_mutex, NULL);
        if (res9 != 0)
            printf("error check_for_death_mutex has been created mutex failed\n");
        else
            printf("check_for_death_mutex has been created mutex a bien ete cree\n");

    int res29 = pthread_mutex_init(&program->clock_mutex, NULL);
    if (res29 != 0)
        printf("error clock_mutex has been created mutex failed\n");
    else
        printf("clock_mutex has been created mutex a bien ete cree\n");

    int res39 = pthread_mutex_init(&program->protection_mutex, NULL);
    if (res39 != 0)
        printf("error protection_mutex has been created mutex failed\n");
    else
        printf("protection_mutex has been created mutex a bien ete cree\n");

     int res49 = pthread_mutex_init(&program->other, NULL);
    if (res49 != 0)
        printf("error other has been created mutex failed\n");
    else
        printf("other has been created mutex a bien ete cree\n");
    
     int res59 = pthread_mutex_init(&program->philo_state_mutex, NULL);
    if (res59 != 0)
        printf("error other has been created mutex failed\n");
    else
        printf("other has been created mutex a bien ete cree\n");

       int res69 = pthread_mutex_init(&program->access_mutex, NULL);
    if (res69 != 0)
        printf("error other has been created mutex failed\n");
    else
        printf("other has been created mutex a bien ete cree\n");

    int res79 = pthread_mutex_init(&program->again_mutex, NULL);
    if (res79 != 0)
        printf("error other has been created mutex failed\n");
    else
        printf("other has been created mutex a bien ete cree\n");
    
    return (program);
}

t_program *init_program(char **argv)
{   
    t_program *program;
    
    program = malloc(sizeof(t_program));
    if (!program)
        return (NULL);
    program->phil_count = ft_atol(argv[1]);
    program->all_philo_created = 0;
    program->all_full = 0;
    program->start_of_program = 0;
    program->end_of_program_flag = 0;
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
    fill_philo_struct(program, argv);
    return (program);
}


long	get_milli(int nb)
{
	nb *= 1000;
	return (nb);
}

void	precise_usleep(long time_to_do_smthg)
{
	struct timeval	s;
	struct timeval	cur;
	long			tps_ecoule;
	long			reste;

	gettimeofday(&s, NULL);
	while (1)
	{
		gettimeofday(&cur, NULL);
		tps_ecoule = (cur.tv_sec - s.tv_sec) * 1e6 + (cur.tv_usec - s.tv_usec);
		if (tps_ecoule >= time_to_do_smthg)
			break ;
		reste = time_to_do_smthg - tps_ecoule;
		if (reste > 1e4)
			usleep(reste / 2);
		else
			while (1)
			{
				gettimeofday(&cur, NULL);
				tps_ecoule = (cur.tv_sec - s.tv_sec) * 1e6 + (cur.tv_usec
						- s.tv_usec);
				if (tps_ecoule >= time_to_do_smthg)
					break ;
			}
	}
}

void update_meal_counter(t_philo *philo) 
{
    struct timeval	tv;

	t_program *program;

	program = philo->ptr;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&program->new_update_meal_counter_mutex);
    philo->meal_counter++;
	philo->last_meal_time = (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex);
}

int simulation_finished(t_program *program)
{
    int res;
    
    pthread_mutex_lock(&program->protection_mutex);
    res = program->end_of_program_flag;
    // printf("res= %d\n", res);
    pthread_mutex_unlock(&program->protection_mutex);
    return (res);
}


void write_monitor(t_program *program, t_philo *philo)
{
    int state;
    struct timeval tv;
    long clock;

    pthread_mutex_lock(&program->printf_mutex);
    pthread_mutex_lock(&program->philo_state_mutex); // Verrouillage de l'accès à la variable state
    // state = (*(philo->state));
    state = *(program->philosophers[philo->id].state);
    pthread_mutex_unlock(&program->philo_state_mutex); // Déverrouillage de l'accès à la variable state
    gettimeofday(&tv, NULL);
    clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

    if (state == THINKING)
        printf("%ld %d is thinking\n", clock, philo->id + 1);
    else if (state == HAS_TAKEN_A_FORK)
        printf("%ld %d has taken a fork\n", clock, philo->id + 1);
    else if (state == EATING)
        printf("%ld %d is eating\n", clock, philo->id + 1);
    else if (state == SLEEPING)
        printf("%ld %d is sleeping\n", clock, philo->id + 1);
    else if (state == DEAD)
    {
        printf("%ld %d is dead\n", clock, philo->id + 1);
        // exit (0);
    }
    else
        printf("Dinner is over\n");
    pthread_mutex_unlock(&program->printf_mutex);
}


// dans actions pick up forks
void check_if_full(t_philo *philo)
{
    t_program *program;

    program = philo->ptr;
    
    pthread_mutex_lock(&program->new_update_meal_counter_mutex);
    if (philo->meal_counter == philo->nb_of_time_to_eat)
        philo->full = 1;
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex);
}

// void	pickup_forks(t_program *program, t_philo *philo)
// {
// 	int	left_fork_id;
// 	int	right_fork_id;

// 	left_fork_id = philo->id;
// 	right_fork_id = (philo->id + 1) % program->phil_count;
	
// 	if (simulation_finished(program) == 0)
// 	{
// 		if ((philo->id + 1) - program->phil_count == 0)
// 		{
// 			pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
// 			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
// 			write_monitor(program, philo);
		
// 			pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
// 			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
// 			write_monitor(program, philo);
// 		}
// 		else
// 		{
// 			pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
// 			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
// 			write_monitor(program, philo);
			
// 			pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
// 			*(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
// 			write_monitor(program, philo);
// 		}
// 		update_meal_counter(philo);
// 		check_if_full(philo);
// 		*(program->philosophers[philo->id].state) = EATING;
// 		write_monitor(program, philo);
// 	}
// }


void	putdown_forks(t_program *program, t_philo *philo)
{
	struct timeval	tv;
     long clock;
	int				left_fork_id;
	int				right_fork_id;

	gettimeofday(&tv, NULL);
    clock = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
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
	// pthread_mutex_lock(&program->philo_state_mutex);
	*(program->philosophers[philo->id].state) = SLEEPING;
    // printf("%ld %d is sleeping\n", clock, philo->id + 1);
	// pthread_mutex_unlock(&program->philo_state_mutex);
	write_monitor(program, philo);

}



void	wait_for_all(t_program *program)
{
	pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	while (program->all_philo_created != 1)
	{
		pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
		precise_usleep(100000);
		pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	}
	pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
}


// pour voir si j arrete mon dinner car nb of philo full reached
int new_nb_of_full_philos(t_program *program)
{
    int count;
    int i;
    
    count = 0;
    pthread_mutex_lock(&program->all_philo_full_mutex);
    i = 0;
    while (i < program->phil_count)
    {
        pthread_mutex_lock(&program->new_update_meal_counter_mutex);
        if (program->philosophers[i].full == 1)
            count++;
        pthread_mutex_unlock(&program->new_update_meal_counter_mutex);
        i++;
    }
    pthread_mutex_unlock(&program->all_philo_full_mutex);
    return (count);
}

int philo_is_dead(t_philo *philo)
{
    int status;
    t_program *program;
    struct timeval tv;
    long current_time;
    
    status = 0;
    program = philo->ptr;
    
    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;

    pthread_mutex_lock(&program->new_update_meal_counter_mutex); 
    if (philo->last_meal_time == -1)
        philo->last_meal_time = current_time;
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex); 
        
    pthread_mutex_lock(&program->new_update_meal_counter_mutex);
  
    if ((current_time - philo->last_meal_time) > (philo->time_to_die))
    {
        // pthread_mutex_lock(&program->philo_state_mutex);
        *(program->philosophers[philo->id].state) = DEAD; 
        write_monitor(program, philo);
       //  printf("%ld %d is dead\n", current_time, philo->id + 1);
        // pthread_mutex_unlock(&program->philo_state_mutex);
        status = 1;
 
    }
    pthread_mutex_unlock(&program->new_update_meal_counter_mutex);

    return status;
}

void *monitor_program(void *data)
{
    t_program *program;
    
    program = (t_program *)data;

    wait_for_all(program);

    while (!program->end_of_program_flag)
    {
        int i = 0;
        int j;
        while (i < program->phil_count && !simulation_finished(program))
        {    
            if (philo_is_dead(&program->philosophers[i]) == 1)
            {
                // printf("info transmise\n");
                pthread_mutex_lock(&program->again_mutex);
                j = 0;
                while (j < program->phil_count)
                {
                    program->philosophers[j].must_stop = 1;
                    j++;
                }
                pthread_mutex_unlock(&program->again_mutex);
            }
            i++;
        }
    }
    return (NULL);
}

int end_of_program(t_program *program)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {    
        if (philo_is_dead(&program->philosophers[i]) == 1)
        {
            pthread_mutex_lock(&program->protection_mutex);
            program->end_of_program_flag = 1;
            // printf("flag leve\n");
            pthread_mutex_unlock(&program->protection_mutex);
            return 1;
        }
        i++;
    }
    return 0;
}


void *thread_routine(void *data) 
{

    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;

    wait_for_all(program);
    

    while (end_of_program(program) != 1 && philo->must_stop == 0)
    { 
        // printf("phi id = %d check = %d\n", philo->id, philo->must_stop);
        if (new_nb_of_full_philos(program) == program->phil_count)
            break ;

        if (simulation_finished(program) == 1)
            break ;
        
        // pickup_forks(program, philo);
        // -------------------------

        int	left_fork_id;
        int	right_fork_id;

        left_fork_id = philo->id;
        right_fork_id = (philo->id + 1) % program->phil_count;
        
        if (philo->must_stop == 0)
        {
                if ((philo->id + 1) - program->phil_count == 0)
            {
                pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
                pthread_mutex_lock(&program->philo_state_mutex);
                *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
                pthread_mutex_unlock(&program->philo_state_mutex);

                write_monitor(program, philo);
            
                pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
                pthread_mutex_lock(&program->philo_state_mutex);
                *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
                pthread_mutex_unlock(&program->philo_state_mutex);
                write_monitor(program, philo);
            }
            else
            {
                pthread_mutex_lock(&program->forks_mutex[left_fork_id]);
                pthread_mutex_lock(&program->philo_state_mutex);
                *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
                pthread_mutex_unlock(&program->philo_state_mutex);
                write_monitor(program, philo);
                
                pthread_mutex_lock(&program->forks_mutex[right_fork_id]);
                pthread_mutex_lock(&program->philo_state_mutex);
                *(program->philosophers[philo->id].state) = HAS_TAKEN_A_FORK;
                pthread_mutex_unlock(&program->philo_state_mutex);
                write_monitor(program, philo);
            }
            update_meal_counter(philo);
            check_if_full(philo);
            pthread_mutex_lock(&program->philo_state_mutex);
            *(program->philosophers[philo->id].state) = EATING;
            pthread_mutex_unlock(&program->philo_state_mutex);
            write_monitor(program, philo);


            // ----------------------
            precise_usleep(get_milli(philo->time_to_eat));

            putdown_forks(program, philo);
            precise_usleep(get_milli(philo->time_to_sleep));
        
            pthread_mutex_lock(&program->philo_state_mutex);
            *(program->philosophers[philo->id].state) = THINKING;
            pthread_mutex_unlock(&program->philo_state_mutex);
            // pthread_mutex_lock(&program->access_mutex);
            write_monitor(program, philo);
            // pthread_mutex_unlock(&program->access_mutex);
        }
        else  
            printf("coicoucoucoucoucouocucoucoucocuoucoucoucoucouc\n");
        
    }   
    return NULL;
}

static void	create_philos(t_program *program)
{
	int	i;
	int	ret;

	i = 0;
	while (i < program->phil_count)
	{
		ret = pthread_create(&program->philosophers[i].thread_id, NULL,
				thread_routine, (void *)&program->philosophers[i]);
		if (ret != 0)
		{
			printf("Erreur lors de la creation des threads\n");
			exit(0);
		}
		pthread_mutex_lock(&program->one_philo_has_been_created_mutex);
		program->philosophers[i].has_been_created = 1;
		pthread_mutex_unlock(&program->one_philo_has_been_created_mutex);
		i++;
	}
}

static void	create_monitor(t_program *program)
{
	int	ret;

	ret = pthread_create(&program->monitor, NULL,
			monitor_program, (void *)program);
	if (ret != 0)
	{
		printf("Erreur lors de la creation des threads\n");
		exit(0);
	}
	else
		printf("monitor programm cree ok\n");
		
}

// dans create_threads lorsqu un philo est cree son status passe a 1, ici je
// recupere le count des threads crees

static int	nb_of_philo_created(t_program *program)
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

static void	wait_for_everybody_and_set_starting_time(t_program *program)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&program->all_philos_have_been_created_mutex);
	while (nb_of_philo_created(program) != program->phil_count)
		precise_usleep(1000);
	program->all_philo_created = 1;
	printf("all philo have been created\n");
	program->start_of_program = tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
	pthread_mutex_unlock(&program->all_philos_have_been_created_mutex);
}



void clean_program(t_program *program)
{
    int i;

    i = 0;
    while (i < program->phil_count)
    {
        pthread_mutex_destroy(&program->forks_mutex[i]);
        // pthread_mutex_destroy(&program->philosophers[i].update_meal_counter_mutex);
        pthread_mutex_destroy(&program->philosophers[i].full_mutex);
        free(program->philosophers[i].state);
        i++;
    }
    pthread_mutex_destroy(&program->one_philo_has_been_created_mutex);
    pthread_mutex_destroy(&program->all_philos_have_been_created_mutex);
    pthread_mutex_destroy(&program->one_philo_is_full_mutex);
    pthread_mutex_destroy(&program->all_philo_full_mutex);
    // pthread_mutex_destroy(&program->nb_of_full_philo_mutex);
    // pthread_mutex_destroy(&program->all_full_mutex);
    // pthread_mutex_destroy(&program->check_for_death_mutex);
    pthread_mutex_destroy(&program->printf_mutex);
    pthread_mutex_destroy(&program->protection_mutex);
    // free(program->printf_mutex);
    free(program->forks_mutex);
    free(program->philosophers);
    free(program);
}


static void	join_threads(t_program *program)
{
	int	i;
	int	ret;

 
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
    
    // pthread_join(program->monitor, NULL);
}

void	handle_threads(t_program *program)
{
	create_philos(program);
	create_monitor(program);
	wait_for_everybody_and_set_starting_time(program);
	join_threads(program);
    // program->end_of_program_flag = 1;
    pthread_join(program->monitor, NULL);

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
    
    program = init_program(argv);
    program = handle_program_mutexes(program);

    // int i = 0; 
    // while (i < program->phil_count) 
    // {
    //     printf("\nPhilosopher %d:\n", i + 1);
    //     print_philo_struct(&program->philosophers[i]);
    //     i++;
    // }
    // printf("\n\n");
    
    handle_threads(program);
    clean_program(program);
	return (0);
}

// pb avec la mort
// pb du cas ou meal count == 0 - probablement faire 2 fonctions pour la mort et checker fill_philo_struct
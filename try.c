#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    pthread_t thread_id;
    int has_been_created;
        void                *ptr;
    // Autres attributs des philosophes
} t_philo;

typedef struct {
    t_philo *philosophers;
    int phil_count;
    int all_philo_created;
    pthread_mutex_t has_been_created_mutex;
    pthread_mutex_t all_philo_created_mutex;
    pthread_mutex_t syncro_mutex;
} t_program;

int nb_of_philo_ready(t_program *program) {
    int ready_count = 0;
    for (int i = 0; i < program->phil_count; i++) {
        pthread_mutex_lock(&program->has_been_created_mutex);
        if (program->philosophers[i].has_been_created)
            ready_count++;
        pthread_mutex_unlock(&program->has_been_created_mutex);
    }
    return ready_count;
}

void *thread_routine(void *data) {
    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;

    printf("before while loop\n");
    pthread_mutex_lock(&program->all_philo_created_mutex);
    while (program->all_philo_created != 1)
        pthread_mutex_unlock(&program->all_philo_created_mutex);
    printf("coucou\n");
    pthread_mutex_unlock(&program->all_philo_created_mutex);

    return NULL;
}

void create_philo_threads(t_program *program) {
    int i;
    int ret;
    struct timeval tv;

    pthread_mutex_lock(&program->syncro_mutex);
    program->all_philo_created = 0; // initialisation
    pthread_mutex_unlock(&program->syncro_mutex);

    i = 0;
    while (i < program->phil_count) {
        ret = pthread_create(&program->philosophers[i].thread_id, NULL, thread_routine, (void *)&program->philosophers[i]);
        if (ret != 0) {
            printf("Erreur lors de la creation des threads\n");
            exit(0);
        }
        pthread_mutex_lock(&program->has_been_created_mutex); 
        program->philosophers[i].has_been_created = 1;
        pthread_mutex_unlock(&program->has_been_created_mutex); 
        i++;
    }

    // Vérifiez si tous les threads ont été créés
    pthread_mutex_lock(&program->syncro_mutex);
    while (nb_of_philo_ready(program) != program->phil_count)
        usleep(1000);
    pthread_mutex_unlock(&program->syncro_mutex);

    // Mettez à jour all_philo_created après la création de tous les threads
    pthread_mutex_lock(&program->all_philo_created_mutex);
    program->all_philo_created = 1;
    pthread_mutex_unlock(&program->all_philo_created_mutex);
    printf("all philos have been created - updating program->all_philo_created = 1\n");
}



int main() {
    // Initialisez et allouez de la mémoire pour le programme et les philosophes
    t_program *program = malloc(sizeof(t_program));
    program->phil_count = 5;
    program->philosophers = malloc(program->phil_count * sizeof(t_philo));

    // Initialisez les mutex
    pthread_mutex_init(&program->has_been_created_mutex, NULL);
    pthread_mutex_init(&program->all_philo_created_mutex, NULL);
    pthread_mutex_init(&program->syncro_mutex, NULL);

    // Créez les threads des philosophes
    create_philo_threads(program);

    // Attendez la fin des threads des philosophes
    for (int i = 0; i < program->phil_count; i++)
        pthread_join(program->philosophers[i].thread_id, NULL);

    // Libérez la mémoire et détruisez les mutex
    free(program->philosophers);
    free(program);
    pthread_mutex_destroy(&program->has_been_created_mutex);
    pthread_mutex_destroy(&program->all_philo_created_mutex);
    pthread_mutex_destroy(&program->syncro_mutex);

    return 0;
}


pthread_mutex_t has_been_created_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t all_philo_created_mutex = PTHREAD_MUTEX_INITIALIZER;

int nb_of_philo_ready(t_program *program) 
{
    pthread_mutex_lock(&has_been_created_mutex);
    int count = 0;
    for (int i = 0; i < program->phil_count; i++) {
        if (program->philosophers[i].has_been_created == 1)
            count++;
    }
    pthread_mutex_unlock(&has_been_created_mutex);
    return count;
}


void *thread_routine(void *data) 
{
    t_philo *philo = (t_philo *)data;
    t_program *program = (t_program *)philo->ptr;

    

    pthread_mutex_lock(&all_philo_created_mutex);
    while (program->all_philo_created != 1) 
    {
        pthread_mutex_unlock(&all_philo_created_mutex);
        printf("Philosophe %d: ATTEND que tous les philosophes soient créés\n", philo->id);
        usleep(1000); // Attente pour éviter un blocage actif
        pthread_mutex_lock(&all_philo_created_mutex);
    }
    pthread_mutex_unlock(&all_philo_created_mutex);

    printf("Philosophe %d: après que tous les philosophes soient créés\n", philo->id);

    return NULL;
}

void create_philo_threads(t_program *program) {
    int ret;
    for (int i = 0; i < program->phil_count; i++) {
        ret = pthread_create(&program->philosophers[i].thread_id, NULL, thread_routine, (void *)&program->philosophers[i]);
        if (ret != 0) {
            printf("Erreur lors de la creation des threads\n");
            exit(0);
        }
        pthread_mutex_lock(&has_been_created_mutex); 
        program->philosophers[i].has_been_created = 1;
        pthread_mutex_unlock(&has_been_created_mutex); 
        printf("Philosophe %d créé\n", i);
    }

    pthread_mutex_lock(&program->syncro_mutex);
    while (nb_of_philo_ready(program) != program->phil_count)
        usleep(1000);
    pthread_mutex_unlock(&program->syncro_mutex);

    pthread_mutex_lock(&all_philo_created_mutex);
    program->all_philo_created = 1;
    printf("all philos have been created - updating program->all_philo_created = 1\n");
    pthread_mutex_unlock(&all_philo_created_mutex);

    for (int i = 0; i < program->phil_count; i++) {
        ret = pthread_join(program->philosophers[i].thread_id, NULL);
        if (ret != 0) {
            printf("Erreur lors de la jonction des threads\n");
            exit(0);
        }
    }
}

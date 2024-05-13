/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:11:16 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/13 17:03:53 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor_routine(void *data) 
{
    long test;
    test = (long)data; // Cast du pointeur data à son type d'origine (long)

    printf("hey %ld\n", test); // Utilisation de la variable castée pour l'impression
    

    // t_program *program;

    // program = (t_program *)data;
    // pthread_mutex_lock(&program->shared_data_mutex);
    
    // wait_for_all(program);
    // while (program->start_of_program == 0)
    //     ;
    // sleep(4);
    // printf("coucou\n");
    
    // pthread_mutex_unlock(&program->shared_data_mutex);

    return NULL;
}

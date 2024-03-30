/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parisng.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 10:29:33 by malanglo          #+#    #+#             */
/*   Updated: 2024/03/30 11:29:48 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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













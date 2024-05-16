/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:58:57 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 09:18:53 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

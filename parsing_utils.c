/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:59:17 by malanglo          #+#    #+#             */
/*   Updated: 2024/05/15 09:19:06 by malanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

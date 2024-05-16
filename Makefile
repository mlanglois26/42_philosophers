# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 12:00:22 by malanglo          #+#    #+#              #
#    Updated: 2024/05/16 12:15:14 by malanglo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc 

CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = 			parsing_utils.c\
				parsing.c\
				init_mutex.c\
				init.c\
				threads_utils.c\
				threads.c\
				time.c\
				helper_ph_routine.c\
				philo_routine.c\
				monitor_routine.c\
				monitor_utils_full.c\
				monitor_utils_death.c\
				write_actions.c\
				fork_actions.c\
				main.c\
				
OBJS =		$(SRCS:.c=.o)

all: $(NAME)

$(NAME):		${OBJS}
		$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

clean:
		rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re



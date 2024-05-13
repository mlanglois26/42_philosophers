# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malanglo <malanglo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 12:00:22 by malanglo          #+#    #+#              #
#    Updated: 2024/05/13 17:55:11 by malanglo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc 

CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = 			parsing_utils.c\
				parsing.c\
				init.c\
				threads.c\
				time.c\
				philo_routine.c\
				monitor_routine.c\
				actions.c\
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



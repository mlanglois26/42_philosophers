
NAME = philo

CC = cc 

CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = 			parsing_utils.c\
				parsing.c\
				init.c\
				routine_utils.c\
				time.c\
				actions.c\
				routine.c\
				threads.c\
				main.c
				
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
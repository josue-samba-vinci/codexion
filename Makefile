NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I.
LDFLAGS		= -pthread

HEADER		= codexion.h

SRCS		= main.c \
			  parser.c \
			  init.c \
			  utils/ft_atoi.c \
			  utils/ft_strcmp.c \
			  utils/parser_utils.c \
			  utils/time.c \
			  utils/log.c \
			  simulation.c \
			  dongle.c

OBJS		= $(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o:		%.c $(HEADER)
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re

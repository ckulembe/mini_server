NAME		=	mini_serv
CC		=	cc
CFLAGS	=	-g -Wall -Wextra -Werror

SRC		=	mini_serv.c
OBJS		=	$(SRC:.c=.o)
RM		=	rm -f

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC)	$(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all re clean fclean
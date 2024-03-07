NAME		= pipex

CC 			= cc

CFLAGS		= -g3 -Wall -Wextra -Werror

SRC			= main.c access_cmd1.c access_cmd2.c child_cmd1.c child_cmd2.c utils.c

OBJ 		= $(SRC:.c=.o)

all : $(NAME)

%.o : %.c pipex.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	make -sC ft_printf
	make -sC libft
	$(CC) $(OBJ) -L ft_printf -l ftprintf -L libft -l ft -o $(NAME)

clean :
	rm -rf  $(OBJ) 
	make -sC ft_printf clean
	make -sC libft clean

fclean : clean
		rm -f $(NAME)
		make -sC ft_printf fclean
		make -sC libft fclean

re : fclean all

.PHONY : all clean fclean re

NAME = libft.a
C_Compiler = cc
C_FLAGS = -Wall -Wextra -Werror

Funs = 	ft_strrchr.c ft_strcmp.c ft_atoi.c

OFuns = $(Funs:.c=.o)

all: $(NAME)

$(NAME): $(OFuns)
	ar rc ${NAME} $(OFuns)
	
%.o: %.c libft.h
	$(C_Compiler) $(C_FLAGS) -c $< -o $@

clean:
	$(RM) $(OFuns)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean

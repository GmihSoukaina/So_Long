COMPILER = cc
CFLAGS = -Wall -Wextra -Werror

RED = \033[0;31m
GREEN = \033[0;32m
RESET = \033[0m

MLX = -lmlx -framework OpenGL -framework AppKit

Funs =	so_long_Funs/so_long.c \
		so_long_Funs/so_long_utils.c \
		so_long_Funs/funs_helper.c \
		so_long_Funs/parsing.c \
		lib/gnl/get_next_line_utils.c \
		lib/gnl/get_next_line.c

OFuns = ${Funs:.c=.o}

NAME = so_long

LIBFT = lib/libft/libft.a

HEADER = so_long_Funs/so_long.h \
		 lib/gnl/get_next_line.h \
		 lib/libft/libft.h

all: ${NAME}

${NAME} : ${OFuns} ${LIBFT}
	@${COMPILER} ${CFLAGS} ${OFuns} ${LIBFT} ${MLX} -o ${NAME}
	@echo "${GREEN}✅ ${NAME} successfully compiled!${RESET}"


so_long_Funs/%.o: so_long_Funs/%.c ${HEADER}
	@${COMPILER} ${CFLAGS} -c $< -o $@

${LIBFT}:
	@make -C lib/libft

clean:
	@${RM} ${OFuns} $(OBJB)
	@make clean -C lib/libft
	@echo "${RED}🗑 Object files removed!${RESET}"

fclean: clean
	@${RM} ${NAME} ${BONUS_NAME}
	@make fclean -C lib/libft
	@echo "${RED}🔥 Executable removed!${RESET}"

re: fclean all

.PHONY: clean
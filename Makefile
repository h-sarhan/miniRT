# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 14:01:09 by hsarhan           #+#    #+#              #
#    Updated: 2022/11/21 12:55:28 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PARSING_SRC = parse_utils.c shape_errors.c scene_errors.c \
				parse_scene.c parse_shapes.c parse_elements.c parse_attributes.c
PARSING_SRC := $(addprefix parsing/, $(PARSING_SRC))

MATH_SRC = vector_arithmetic.c vector_operations.c matrix_operations.c matrix_inverse.c
MATH_SRC := $(addprefix math/, $(MATH_SRC))

SRC = $(PARSING_SRC) $(MATH_SRC) free_utils.c print_utils.c

SRC := $(addprefix src/, $(SRC))

OBJ_DIR = .obj
OBJ := $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJ += .obj/src/main.o

LIBFT = libft/libft.a

NAME = miniRT

CC = gcc
INC = -Iinclude -Ilibft

OPTIMIZATION_FLAGS = -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops

CFLAGS = -Wall -Wextra  -g3  $(INC) \
			-fsanitize=address \
			# $(OPTIMIZATION_FLAGS) \


all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBFT):
	make -j10 -C libft

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

leakcheck: $(LIBFT) $(OBJ)
	rm -f $(NAME)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT)  -Lleaksan -llsan -lc++ -o $(NAME)

clean:
	make -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(LIBFT)

norm:
	-make -C libft norm
	norminette src include

re: fclean all

.PHONY: all re fclean clean leak
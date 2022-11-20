# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 14:01:09 by hsarhan           #+#    #+#              #
#    Updated: 2022/11/20 16:33:45 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PARSING_SRC = parsing_utils.c parsing_utils2.c shape_errors.c scene_errors.c \
				parse_scene.c parse_shapes.c parse_elements.c parse_attributes.c
PARSING_SRC := $(addprefix parsing/, $(PARSING_SRC))

SRC = $(PARSING_SRC) free_utils.c print_utils.c

SRC := $(addprefix src/, $(SRC))

OBJ_DIR = .obj
OBJ := $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJ += .obj/src/main.o

LIBFT = libft/libft.a

NAME = miniRT

CC = gcc
INC = -Iinclude -Ilibft

OPTIMIZATION_FLAGS = -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops

CFLAGS = -Wall -Wextra -Werror -g3  $(INC) \
			-fsanitize=address \
			# $(OPTIMIZATION_FLAGS) \


all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBFT):
	make -j10 -C libft

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -Lleaksan -llsan -lc++ -o $(NAME)

clean:
	make -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(LIBFT)

norm:
	-make -C libft norm
	norminette src

re: fclean all

.PHONY: all re fclean clean
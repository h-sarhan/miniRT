# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 14:01:09 by hsarhan           #+#    #+#              #
#    Updated: 2022/11/18 17:56:07 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PARSING_SRC = parsing.c
PARSING_SRC := $(addprefix parsing/, $(PARSING_SRC))

SRC = $(PARSING_SRC) color.c

SRC := $(addprefix src/, $(SRC))

OBJ_DIR = .obj
OBJ := $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJ += .obj/src/main.o

LIBFT = libft/libft.a

NAME = miniRT

CC = gcc
INC = -Iinclude -Ilibft

OPTIMIZATION_FLAGS = -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops

CFLAGS = -Wall -Wextra -g3 $(OPTIMIZATION_FLAGS) $(INC) \
			-fsanitize=address


all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBFT):
	make -j10 -C libft

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

clean:
	make -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(LIBFT)

re: fclean all

.PHONY: all re fclean clean
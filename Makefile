# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 14:01:09 by hsarhan           #+#    #+#              #
#    Updated: 2023/01/02 13:10:24 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PARSING_SRC = parse_utils.c shape_errors.c scene_errors.c \
				parse_scene.c parse_shapes.c parse_elements.c parse_attributes.c \
				parse_settings.c

PARSING_SRC := $(addprefix parsing/, $(PARSING_SRC))

MATH_SRC = vector_arithmetic.c vector_operations.c matrix_operations.c matrix_inverse.c\
				matrix_inverse2.c matrix_transformations.c
MATH_SRC := $(addprefix math/, $(MATH_SRC))

INTERSECTION_SRC = intersections.c reflections.c sort_intersections.c refraction.c
INTERSECTION_SRC := $(addprefix intersection/, $(INTERSECTION_SRC))

LIGHTING_SRC = lighting.c camera.c
LIGHTING_SRC := $(addprefix lighting/, $(LIGHTING_SRC))

EDIT_MODE_SRC = key_presses.c loop_hook.c mouse_controls.c collisions.c 
EDIT_MODE_SRC := $(addprefix edit_mode/, $(EDIT_MODE_SRC))

SRC = $(PARSING_SRC) $(MATH_SRC) $(INTERSECTION_SRC) $(LIGHTING_SRC) $(EDIT_MODE_SRC) \
				free_utils.c print_utils.c color.c draw_scene.c render_scene.c \
				main.c utils.c ui.c

SRC := $(addprefix src/, $(SRC))

OBJ_DIR = .obj
OBJ := $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
DEPENDS := $(OBJ:.o=.d)

LIBFT = libft/libft.a
NAME = miniRT

OS := $(shell uname)
CC = gcc
ifeq ($(OS),Linux)
	INC = -Iinclude -Ilibft -Imlx -I/usr/include -Imlx_linux
	OPTIMIZATION_FLAGS = -O3 -march=native -flto -fno-signed-zeros -funroll-loops
	LINK_FLAGS = -Lmlx_linux -lmlx -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
	MLX = mlx_linux
else
	INC = -Iinclude -Ilibft -Imlx 
	OPTIMIZATION_FLAGS = -Ofast -march=native -flto -fno-signed-zeros -funroll-loops
	LINK_FLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit 
	MLX = mlx
endif


CFLAGS = -Wall -Wextra -Werror -march=native -ggdb -pthread $(INC) \
			$(OPTIMIZATION_FLAGS) \
			# -fsanitize=address\

all:
	# @make -j20 $(NAME)
	@make  $(NAME)

$(OBJ_DIR)/%.o: %.c 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

$(LIBFT):
	make -C  libft

$(NAME): $(LIBFT) $(OBJ) Makefile
	-make -s  all -C $(MLX)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LINK_FLAGS) -o $(NAME)

-include $(DEPENDS)

clean:
	-make -C $(MLX) clean
	-make -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(LIBFT)

norm:
	-make -C libft norm
	norminette src include

re: fclean all

.PHONY: all re fclean clean

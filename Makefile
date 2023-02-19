# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 14:01:09 by hsarhan           #+#    #+#              #
#    Updated: 2023/02/19 15:12:23 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EDIT_MODE_SRC = key_presses.c loop_hook.c mouse_controls.c mouse_utils.c
EDIT_MODE_SRC := $(addprefix edit_mode/, $(EDIT_MODE_SRC))

INTERSECTIONS_SRC = normal.c intersections.c cone_intersection.c \
					cylinder_intersection.c normal_map.c
INTERSECTIONS_SRC := $(addprefix intersections/, $(INTERSECTIONS_SRC))

MATH_SRC = vector_arithmetic.c vector_operations.c matrix_operations.c \
				matrix_inverse.c matrix_inverse2.c transformation_matrices.c
MATH_SRC := $(addprefix math/, $(MATH_SRC))

PARSING_SRC =   errors.c shape_errors.c  attribute_errors.c element_errors.c\
				parse_scene.c parse_shapes1.c parse_shapes2.c parse_elements.c \
				parse_attributes.c parse_settings.c settings_errors.c \
				settings_validation.c parse_texture.c

PARSING_SRC := $(addprefix parsing/, $(PARSING_SRC))

RENDERER_SRC = camera.c fill_pixels.c render_scene.c \
				transforms.c workers.c
RENDERER_SRC := $(addprefix renderer/, $(RENDERER_SRC))

SHADING_SRC = color_operations.c phong.c reflections.c patterns.c
SHADING_SRC := $(addprefix shading/, $(SHADING_SRC))

UI_SRC = arrow.c loading_bar.c marker.c shape_info.c help_menu.c
UI_SRC := $(addprefix ui/, $(UI_SRC))

UTILS_SRC = color_utils.c free_utils.c intersection_utils.c math_utils.c \
		print_utils.c parse_utils.c 
UTILS_SRC := $(addprefix utils/, $(UTILS_SRC))


SRC = $(EDIT_MODE_SRC) $(INTERSECTIONS_SRC) $(MATH_SRC) $(PARSING_SRC) \
	$(RENDERER_SRC) $(SHADING_SRC) $(UI_SRC) $(UTILS_SRC) main.c
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
	OPTIMIZATION_FLAGS = -Ofast -march=native -flto -fno-signed-zeros -funroll-loops
	LINK_FLAGS = -Lmlx_linux -lmlx -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
	MLX = mlx_linux
else
	INC = -Iinclude -Ilibft -Imlx 
	OPTIMIZATION_FLAGS = -Ofast -march=native -flto -fno-signed-zeros -funroll-loops 
	LINK_FLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit 
	MLX = mlx
endif


CFLAGS = -Wall -Wextra -Werror -g3 -pthread $(INC) \
			$(OPTIMIZATION_FLAGS) \
			# -fsanitize=address,undefined\

all:
	# @make -j20 $(NAME)
	make  $(NAME)

$(OBJ_DIR)/%.o: %.c 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

$(LIBFT):
	-make -C  libft

$(NAME): $(LIBFT) $(OBJ) Makefile
	-make  all -C $(MLX)
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

.PHONY: all re fclean clean norm

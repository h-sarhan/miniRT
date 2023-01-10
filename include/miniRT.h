/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:40:23 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/10 14:40:35 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <time.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <semaphore.h>
# include <unistd.h>
# include <pthread.h>
# include "../libft/libft.h"
# ifdef __linux__
#  include "../mlx_linux/mlx.h"
# else
#  include "../mlx/mlx.h"
# endif
# include "mathRT.h"
# include "color.h"
# include "display.h"
# include "shape.h"
# include "camera.h"
# include "edit_mode.h"
# include "intersections.h"
# include "lights.h"
# include "scene.h"
# include "render.h"
# include "parsing.h"
# include "utils.h"

# define EPSILON 0.0001

#endif

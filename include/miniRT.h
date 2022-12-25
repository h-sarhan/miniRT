/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:40:23 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/25 23:52:58 by hsarhan          ###   ########.fr       */
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
# include "../mlx_linux/mlx.h"
#else
# include "../mlx/mlx.h"
#endif
# include "mathRT.h"
# include "camera.h"
# include "color.h"
# include "edit_mode.h"
# include "shape.h"
# include "intersections.h"
# include "lights.h"
# include "render.h"
# include "scene.h"
# include "parsing.h"
# include "utils.h"

# define EPSILON 0.001

#endif
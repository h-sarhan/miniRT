/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:40:23 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/23 12:03:13 by hsarhan          ###   ########.fr       */
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
# include "../libft/libft.h"
# include "../mlx/mlx.h"
# include "intersection.h"
# include "matrix.h"
# include "parsing.h"
# include "scene.h"
# include "utils.h"
# include "vector.h"
# include "key_input.h"
# include "render.h"

# define NUM_THREADS 16
# define REFLECTION_DEPTH 1
# define CAM_SPEED 0.5
# define EPSILON 0.001

#endif
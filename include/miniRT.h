/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:40:23 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/18 12:17:09 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <time.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <immintrin.h>
# include <fcntl.h>
# include <semaphore.h>
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

# define NUM_THREADS 12
# define REFLECTION_DEPTH 1
# define EPSILON 0.001
# define TICK(X) clock_t X = clock()
# define TOCK(X) printf("time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC)

#endif
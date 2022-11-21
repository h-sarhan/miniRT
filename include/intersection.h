/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:19:12 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/21 20:23:16 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERSECTION_H
# define INTERSECTION_H

# include "../mlx/mlx.h"
# include <ray.h>
# include <shape.h>

/**
 * @brief 
 * 
 */
typedef struct s_mlx t_mlx;

struct	s_mlx {
	void	*mlx;
	void	*mlx_win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
};

typedef struct s_intersect t_intersect;

struct s_intersect
{	
	float	time;
	t_shape	*shape;
};

typedef struct s_intersections t_intersections;
struct s_intersections
{
	t_intersect *arr;
	int 		count;
};


void draw_scene(t_scene *scene);

#endif

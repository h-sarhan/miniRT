/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_map2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 21:02:15 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 21:02:24 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	cube_map_back(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->x), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}

void	cubical_map(double *u, double *v, t_vector *point)
{
	double	abs_x;
	double	abs_y;
	double	abs_z;
	double	coord;

	abs_x = fabs(point->x);
	abs_y = fabs(point->y);
	abs_z = fabs(point->z);
	coord = max3(abs_x, abs_y, abs_z);
	if (coord == point->x)
		cube_map_right(u, v, point);
	else if (coord == -point->x)
		cube_map_left(u, v, point);
	else if (coord == point->y)
		cube_map_up(u, v, point);
	else if (coord == -point->y)
		cube_map_down(u, v, point);
	else if (coord == point->z)
		cube_map_front(u, v, point);
	else if (coord == -point->z)
		cube_map_back(u, v, point);
}

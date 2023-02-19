/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_map1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 21:01:39 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 21:01:49 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	cube_map_right(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->z), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}

void	cube_map_left(double *u, double *v, t_vector *point)
{
	*u = (fmod((point->z + 1), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}

void	cube_map_up(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->x), 2.0)) / 2.0;
	*v = (fmod((1 - point->z), 2.0)) / 2.0;
}

void	cube_map_down(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->x), 2.0)) / 2.0;
	*v = (fmod((point->z + 1), 2.0)) / 2.0;
}

void	cube_map_front(double *u, double *v, t_vector *point)
{
	*u = (fmod((point->x + 1), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}

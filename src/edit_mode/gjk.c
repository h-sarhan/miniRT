/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gjk.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:23:32 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/01 15:43:39 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

extern t_vector *point_to_draw_1;
extern t_vector *point_to_draw_2;

int	sign(float num)
{
	if (num < 0)
		return (-1);
	return (1);
}

// This assumes that the cylinder is centered at the origin with default orientation
t_vector	cylinder_support_function(const t_vector *dir, const t_shape *cyl)
{
	(void) dir;
	t_vector	point;
	ft_bzero(&point, sizeof(t_vector));
	point.w = 1;
	point.y = sign(dir->y) * cyl->props.height / 2;
	float	dist_from_center = sqrt(dir->x * dir->x + dir->z * dir->z);
	if (dist_from_center < 0.01)
	{
		return (point);
	}
	point.x = (cyl->props.radius / dist_from_center) * dir->x;
	point.z = (cyl->props.radius / dist_from_center) * dir->z;
	return (point);
}
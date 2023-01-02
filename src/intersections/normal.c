/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:52:03 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 21:48:30 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vector	cylinder_normal(const t_shape *shape, t_vector *point)
{
	float		distance;
	float		cylinder_bottom;
	float		cylinder_top;
	t_vector	normal;

	distance = point->x * point->x + point->z * point->z;
	cylinder_bottom = (shape->props.height / 2);
	cylinder_top = -(shape->props.height / 2);
	if (distance < 1 && (point->y >= cylinder_bottom - EPSILON))
	{
		normal.x = 0;
		normal.y = 1;
		normal.z = 0;
		normal.w = 0;
		return (normal);
	}
	else if (distance < 1 && (point->y <= cylinder_top + EPSILON))
	{
		normal.x = 0;
		normal.y = -1;
		normal.z = 0;
		normal.w = 0;
		return (normal);
	}
	else
	{
		normal.x = point->x;
		normal.y = 0;
		normal.z = point->z;
		normal.w = 0;
		normalize_vec(&normal);
		return (normal);
	}
}

t_vector	cone_normal(const t_shape *shape, t_vector *point)
{
	float		distance;
	float		radius;
	float		cone_bottom;
	float		cone_top;
	t_vector	normal;

	distance = point->x * point->x + point->z * point->z;
	radius = fabs(point->y) * fabs(point->y);
	cone_bottom = (shape->props.height / 2);
	cone_top = 0;
	if (distance < radius && (point->y >= cone_bottom - EPSILON))
	{
		normal.x = 0;
		normal.y = 1;
		normal.z = 0;
		normal.w = 0;
		return (normal);
	}
	else if (distance < radius && (point->y <= cone_top + EPSILON))
	{
		normal.x = 0;
		normal.y = -1;
		normal.z = 0;
		normal.w = 0;
		return (normal);
	}
	else
	{
		distance = sqrt(distance);
		if (point->y > 0)
			distance = -distance;
		normal.x = point->x;
		normal.y = distance;
		normal.z = point->z;
		normal.w = 0;
		normalize_vec(&normal);
		return (normal);
	}
}

t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	object_normal;
	t_vector	world_normal;
	t_vector	local_point;
	t_vector	local_normal;
	float		maxc;

	if (shape->type == SPHERE)
	{
		mat_vec_multiply(&object_normal, &shape->inv_transf, itx_point);
		object_normal.w = 0;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
	}
	if (shape->type == PLANE)
	{
		object_normal.x = 0;
		object_normal.y = 1;
		object_normal.z = 0;
		object_normal.w = 0;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
	}
	if (shape->type == CYLINDER)
	{
		mat_vec_multiply(&local_point, &shape->inv_transf, itx_point);
		local_point.w = 0;
		local_normal = cylinder_normal(shape, &local_point);
		mat_vec_multiply(&world_normal, &shape->norm_transf, &local_normal);
		world_normal.w = 0;
	}
	if (shape->type == CONE)
	{
		mat_vec_multiply(&local_point, &shape->inv_transf, itx_point);
		local_point.w = 0;
		local_normal = cone_normal(shape, &local_point);
		mat_vec_multiply(&world_normal, &shape->norm_transf, &local_normal);
		world_normal.w = 0;
	}
	if (shape->type == CUBE)
	{
		mat_vec_multiply(&object_normal, &shape->inv_transf, itx_point);
		object_normal.w = 0;
		maxc = max3(fabs(object_normal.x),
				fabs(object_normal.y), fabs(object_normal.z));
		if (maxc == fabs(object_normal.x))
		{
			object_normal.y = 0;
			object_normal.z = 0;
		}
		else if (maxc == fabs(object_normal.y))
		{
			object_normal.x = 0;
			object_normal.z = 0;
		}
		else if (maxc == fabs(object_normal.z))
		{
			object_normal.x = 0;
			object_normal.y = 0;
		}
		normalize_vec(&object_normal);
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
	}
	normalize_vec(&world_normal);
	return (world_normal);
}

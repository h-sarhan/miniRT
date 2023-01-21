/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:52:03 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/20 18:55:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vector	cylinder_normal(const t_shape *shape, const t_vector *itx_point)
{
	double		distance;
	t_vector	normal;
	t_vector	point;
	t_vector	world_normal;

	mat_vec_multiply(&point, &shape->inv_transf, itx_point);
	point.w = 0;
	distance = point.x * point.x + point.z * point.z;
	ft_bzero(&normal, sizeof(t_vector));
	if (distance < 1 && (point.y >= (shape->props.height / 2) - EPSILON))
		normal.y = 1;
	else if (distance < 1 && (point.y <= -(shape->props.height / 2) + EPSILON))
		normal.y = -1;
	else
	{
		normal.x = point.x;
		normal.z = point.z;
		normalize_vec(&normal);
	}
	mat_vec_multiply(&world_normal, &shape->norm_transf, &normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}

t_vector	cone_normal(const t_shape *shape, const t_vector *itx_point)
{
	double		distance;
	t_vector	normal;
	t_vector	point;
	t_vector	world_normal;

	mat_vec_multiply(&point, &shape->inv_transf, itx_point);
	distance = point.x * point.x + point.z * point.z;
	ft_bzero(&normal, sizeof(t_vector));
	if (distance < fabs(point.y) * fabs(point.y)
		&& (point.y >= (shape->props.height / 2) - EPSILON))
		normal.y = 1;
	else if (distance < fabs(point.y) * fabs(point.y) && (point.y <= EPSILON))
		normal.y = -1;
	else
	{
		normal.y = sqrt(distance);
		if (point.y > 0)
			normal.y *= -1;
		normal.x = point.x;
		normal.z = point.z;
		normalize_vec(&normal);
	}
	mat_vec_multiply(&world_normal, &shape->norm_transf, &normal);
	normalize_vec(&world_normal);
	return (world_normal);
}

t_vector	plane_normal(const t_shape *shape)
{
	t_vector	object_normal;
	t_vector	world_normal;

	object_normal.x = 0;
	object_normal.y = 1;
	object_normal.z = 0;
	object_normal.w = 0;
	mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}

t_vector	cube_normal(const t_shape *shape, const t_vector *itx_point)
{
	double		maxc;
	t_vector	normal;

	mat_vec_multiply(&normal, &shape->inv_transf, itx_point);
	normal.w = 0;
	maxc = max3(fabs(normal.x), fabs(normal.y), fabs(normal.z));
	if (maxc == fabs(normal.x))
	{
		normal.y = 0;
		normal.z = 0;
	}
	else if (maxc == fabs(normal.y))
	{
		normal.x = 0;
		normal.z = 0;
	}
	else if (maxc == fabs(normal.z))
	{
		normal.x = 0;
		normal.y = 0;
	}
	normalize_vec(&normal);
	return (normal);
}


t_vector	normal_from_texture(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	shape_point;
	double		u;
	double		v;
	t_vector	normal;

	ft_bzero(&normal, sizeof(t_vector));
	// normal.y = 1;
	mat_vec_multiply(&shape_point, &shape->inv_transf, itx_point);
	// if (shape_point.x > 1 || shape_point.y > 1 || shape_point.x < -1 || shape_point.y < -1)
	// 	return (normal);
	// shape_point.y = 1 - shape_point.y;
	
	spherical_map(&u, &v, &shape_point);
	// if (u < 0 || v < 0)
	// 	return (normal);
	// u = (int)(shape_point.x * itx->shape->tex_height);
	// v = (int)(shape_point.y * itx->shape->tex_width);
	u = (int)(u * (shape->tex_height - 1));
	v = (int)(v * (shape->tex_width - 1));
	// if (u >= shape->tex_height || v >= shape->tex_width)
	// 	return (normal);
	t_color	normal_coords = shape->normal_tex[(int)u][(int)v];
	normal.w = 0;
	normal.x = 2 * normal_coords.r - 1;
	normal.y = 2 * normal_coords.g - 1;
	normal.z = normal_coords.b;
	normalize_vec(&normal);
	return (normal);
}

t_vector	normal_at(t_scene *scene, const t_shape *shape, const t_vector *itx_point)
{
	t_vector	normal;
	t_vector	world_normal;

	if (shape->normal_tex != NULL)
	{
		normal = normal_from_texture(shape, itx_point);
		mat_vec_multiply(&world_normal, &scene->cam.inv_trans, &normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
		return (world_normal);
	}
	if (shape->type == SPHERE)
	{
		mat_vec_multiply(&normal, &shape->inv_transf, itx_point);
		normal.w = 0;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
		return (world_normal);
	}
	else if (shape->type == PLANE)
		return (plane_normal(shape));
	else if (shape->type == CYLINDER)
		return (cylinder_normal(shape, itx_point));
	else if (shape->type == CONE)
		return (cone_normal(shape, itx_point));
	normal = cube_normal(shape, itx_point);
	mat_vec_multiply(&world_normal, &shape->norm_transf, &normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}

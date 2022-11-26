/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:07:05 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/26 15:23:39 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ray_position(t_vector *pos, const t_ray *ray, float time)
{
	scale_vec(pos, &ray->direction, time);
	add_vec(pos, pos, &ray->origin);
	pos->w = 1;
}

void	transform_ray(t_ray *transformed_ray, const t_ray *ray, const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf, &ray->origin);
	mat_vec_multiply(&transformed_ray->direction, &shape->inv_transf, &ray->direction);
}

bool	intersect(t_shape *shape, const t_ray *ray, t_intersections *xs)
{
	float		a;
	float		b;
	float		c;
	float		discriminant;
	t_vector	sphere_to_ray;
	t_vector	center;
	t_ray		transf_ray;

	center.x = 0;
	center.y = 0;
	center.z = 0;
	center.w = 1;
	transform_ray(&transf_ray, ray, shape);
	// normalize_vec(&ray->direction);
	sub_vec(&sphere_to_ray, &transf_ray.origin, &center);
	a = dot_product(&transf_ray.direction, &transf_ray.direction);
	b = 2 * dot_product(&transf_ray.direction, &sphere_to_ray);
	c = dot_product(&sphere_to_ray, &sphere_to_ray) - 1;
	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (false);
	b *= -1;
	a *= 2;
	discriminant = sqrtf(discriminant);
	xs->arr[xs->count].time = (b - discriminant) / a;
	xs->arr[xs->count].shape = shape;
	xs->arr[xs->count + 1].time = (b + discriminant) / a;
	xs->arr[xs->count + 1].shape = shape;
	xs->count += 2;
	return (true);
}

t_intersect	*hit(const t_intersections *xs)
{
	float	min_time;
	int		i;
	int		idx;

	i = 0;
	min_time = INFINITY;
	while (i < xs->count)
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)
		{
			min_time = xs->arr[i].time;
			idx = i;
		}
		i++;
	}
	if (min_time == INFINITY)
		return (NULL);
	return (&xs->arr[idx]);
}

t_vector	normal_at(const t_shape *shape, const t_vector *intersection_point)
{
	t_vector	origin;
	t_vector	object_point;
	t_vector	object_normal;
	t_vector	world_normal;
	
	// Object point calculation
	mat_vec_multiply(&object_point, &shape->inv_transf, intersection_point);
	// print_vector(&object_point);
	
	// Object normal calculation
	origin.x = 0;
	origin.y = 0;
	origin.z = 0;
	origin.w = 1;
	sub_vec(&object_normal, &object_point, &origin);
	// object_normal.w = 0;
	
	// World normal calculation
	mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	// print_vector(&world_normal);
	return (world_normal);
}

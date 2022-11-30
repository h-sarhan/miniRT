/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:07:05 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/30 16:08:43 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ray_position(t_vector *pos, const t_ray *ray, double time)
{
	// scale_vec(pos, &ray->direction, time);
	// add_vec(pos, pos, &ray->origin);
	
	// ? More optimized version maybe
	pos->x = ray->direction.x * time + ray->origin.x;
	pos->y = ray->direction.y * time + ray->origin.y;
	pos->z = ray->direction.z * time + ray->origin.z;
	pos->w = 1;
}

void	transform_ray(t_ray *transformed_ray, const t_ray *ray, const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf, &ray->origin);
	mat_vec_multiply(&transformed_ray->direction, &shape->inv_transf, &ray->direction);
}

bool	is_shadowed(t_scene *scene, int	light_idx, t_vector *intersection_point)
{
	t_vector v;
	double	distance;
	t_vector direction;
	t_ray	ray;
	t_intersections arr;
	t_intersect		*intersection;
	unsigned int 	i;

	sub_vec(&v, &scene->lights[light_idx].position, intersection_point);
	distance = vec_magnitude(&v);
	normalize_vec(&v);
	ft_memcpy(&direction, &v, sizeof(t_vector));
	ft_memcpy(&ray.origin, intersection_point, sizeof(t_vector));
	ft_memcpy(&ray.direction, &direction, sizeof(t_vector));
	i = 0;
	arr.count = 0;
	while (i < scene->count.shape_count)
	{
		intersect(&scene->shapes[i], &ray, &arr);
			// break ;
		i++;
	}
	intersection = hit(&arr);
	if (intersection && (intersection->time < distance))
		return(true);
	else
		return(false);
}

bool	intersect(t_shape *shape, const t_ray *ray, t_intersections *xs)
{
	double		a;
	double		b;
	double		c;
	double		discriminant;
	t_vector	sphere_to_ray;
	t_vector	center;
	t_ray		transf_ray;

	center.x = 0;
	center.y = 0;
	center.z = 0;
	center.w = 1;
	transform_ray(&transf_ray, ray, shape);
	if (shape->type == SPHERE)
	{
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
	}
	if (shape->type == PLANE)
	{
		// create local_intersect
		// printf("plane\n");
		if (fabs(transf_ray.direction.y) < 0.001)
			return (false);
		xs->arr[xs->count].time = (transf_ray.origin.y * -1) / transf_ray.direction.y;
		xs->arr[xs->count].shape = shape;
		xs->count++;
	}
	return (true);
}

t_intersect	*hit(t_intersections *xs)
{
	double	min_time;
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
	t_vector	object_point;
	t_vector	world_normal;
	
	if (shape->type == SPHERE)
	{
		mat_vec_multiply(&object_point, &shape->inv_transf, intersection_point);
		object_point.w = 0;
		
		// World normal calculation
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_point);
		world_normal.w = 0;
		normalize_vec(&world_normal);
	}
	if (shape->type == PLANE)
	{
		world_normal.x = 0;
		world_normal.y = 1;
		world_normal.z = 0;
		world_normal.w = 0;
	}
	return (world_normal);
}

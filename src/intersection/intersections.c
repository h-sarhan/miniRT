/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:07:05 by mkhan             #+#    #+#             */
/*   Updated: 2022/12/20 18:35:28 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ray_position(t_vector *pos, const t_ray *ray, double time)
{
	pos->x = ray->direction.x * time + ray->origin.x;
	pos->y = ray->direction.y * time + ray->origin.y;
	pos->z = ray->direction.z * time + ray->origin.z;
	pos->w = 1;
}

void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
	const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf,
		&ray->origin);
	mat_vec_multiply(&transformed_ray->direction, &shape->inv_transf,
		&ray->direction);
}

bool	is_shadowed(t_scene *scene, int light_idx, t_vector *itx_point)
{
	double			distance;
	t_ray			ray;
	t_intersections	arr;
	unsigned int	i;
	t_intersect		*intersection;

	sub_vec(&ray.direction, &scene->lights[light_idx].position, itx_point);
	distance = vec_magnitude(&ray.direction);
	scale_vec(&ray.direction, &ray.direction, 1 / distance);
	ft_memcpy(&ray.origin, itx_point, sizeof(t_vector));
	i = -1;
	arr.count = 0;
	while (++i < scene->count.shapes)
		intersect(&scene->shapes[i], &ray, &arr);
	intersection = hit_skip_transparent(&arr);
	if (intersection && intersection->time < distance)
		return (true);
	return (false);
}

bool	intersect_sphere(t_ray *transf_ray, t_intersections *xs, t_shape *shape)
{
	double		a;
	double		b;
	double		c;
	double		discriminant;

	transf_ray->origin.w = 0;
	a = dot_product(&transf_ray->direction, &transf_ray->direction);
	b = 2 * dot_product(&transf_ray->direction, &transf_ray->origin);
	c = dot_product(&transf_ray->origin, &transf_ray->origin) - 1;
	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (false);
	b *= -1;
	a *= 2;
	discriminant = sqrt(discriminant);
	xs->arr[xs->count].time = (b - discriminant) / a;
	xs->arr[xs->count].shape = shape;
	xs->arr[xs->count + 1].time = (b + discriminant) / a;
	xs->arr[xs->count + 1].shape = shape;
	xs->count += 2;
	return (true);
}

bool	within_cylinder_radius(t_ray *ray, double t)
{
	double x = ray->origin.x + ray->direction.x * t;
	double z = ray->origin.z + ray->direction.z * t;
	if (x * x + z * z <= 1)
		return (true);
	return (false);
}

bool	check_cylinder_caps(t_ray *ray, t_shape *shape, t_intersections *xs)
{
	bool	intersected;

	double cylinder_bottom = (shape->height / 2) ;
	double cylinder_top = -(shape->height / 2);
	intersected = false;
	if (fabs(ray->direction.y) > EPSILON)
	{
		double t;
		t = (cylinder_bottom - ray->origin.y) / ray->direction.y;
		if (within_cylinder_radius(ray, t))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		t = (cylinder_top - ray->origin.y) / ray->direction.y;
		if (within_cylinder_radius(ray, t))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
	}
	return (intersected);
}


bool	intersect(t_shape *shape, const t_ray *ray, t_intersections *xs)
{
	t_ray	transf_ray;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	transform_ray(&transf_ray, ray, shape);
	if (shape->type == SPHERE)
	{
		return (intersect_sphere(&transf_ray, xs, shape));
	}
	if (shape->type == PLANE)
	{
		if (fabs(transf_ray.direction.y) < EPSILON)
			return (false);
		xs->arr[xs->count].time = (transf_ray.origin.y * -1) / \
		transf_ray.direction.y;
		xs->arr[xs->count].shape = shape;
		xs->count++;
	}
	if (shape->type == CYLINDER)
	{
		double cylinder_bottom = (shape->height / 2) + EPSILON ;
		double cylinder_top = - (shape->height / 2) - EPSILON;
		a = transf_ray.direction.x * transf_ray.direction.x + transf_ray.direction.z * transf_ray.direction.z;
		if (fabs(a) < EPSILON)
		{
			return check_cylinder_caps(&transf_ray, shape, xs);
		}
		b = 2 * transf_ray.direction.x * transf_ray.origin.x + 2 * transf_ray.direction.z * transf_ray.origin.z;
		c = transf_ray.origin.x * transf_ray.origin.x + transf_ray.origin.z * transf_ray.origin.z - 1;
		discriminant = b * b - 4 * a * c;
		if (fabs(discriminant) < EPSILON)
		{
			return check_cylinder_caps(&transf_ray, shape, xs);
			// return (false);
		}
		b *= -1;
		a *= 2;
		discriminant = sqrt(discriminant);
		xs->arr[xs->count].time = (b - discriminant) / a;
		xs->arr[xs->count].shape = shape;
		xs->arr[xs->count + 1].time = (b + discriminant) / a;
		xs->arr[xs->count + 1].shape = shape;
		if (xs->arr[xs->count].time > xs->arr[xs->count + 1].time)
			ft_swapd(&xs->arr[xs->count].time, &xs->arr[xs->count + 1].time);
		double y0 = transf_ray.origin.y + xs->arr[xs->count].time * transf_ray.direction.y;
		double y1 = transf_ray.origin.y + xs->arr[xs->count + 1].time * transf_ray.direction.y;
		if ((y0 > cylinder_bottom || y0 < cylinder_top) && (y1 > cylinder_bottom || y1 < cylinder_top))
		{
			// check_cylinder_caps(&transf_ray, shape, xs);
			// return (false);
			return check_cylinder_caps(&transf_ray, shape, xs);
		}
		if (y0 < cylinder_bottom && y0 > cylinder_top)
			xs->count += 1;
		if (y1 < cylinder_bottom && y1 > cylinder_top)
			xs->count += 1;
		check_cylinder_caps(&transf_ray, shape, xs);
		return (true);
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

t_intersect	*hit_skip_transparent(t_intersections *xs)
{
	double	min_time;
	int		i;
	int		idx;

	i = 0;
	min_time = INFINITY;
	while (i < xs->count)
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time && xs->arr[i].shape->transparency <= 0.5)
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

// t_intersect	*hit(t_intersections *xs)
// {
// 	int		i;

// 	i = 0;
// 	while (i < xs->count)
// 	{
// 		if (xs->arr[i].time >= 0)
// 			return (&xs->arr[i]);
// 		i++;
// 	}
// 	return (NULL);
// }

t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	object_normal;
	t_vector	world_normal;

	if (shape->type == SPHERE)
	{
		mat_vec_multiply(&object_normal, &shape->inv_transf, itx_point);
		object_normal.w = 0;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
	}
	if (shape->type == PLANE)
	{
		object_normal.x = 0;
		object_normal.y = 1;
		object_normal.z = 0;
		object_normal.w = 0;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
	}
	if (shape->type == CYLINDER)
	{
		// local_point ← inverse(shape.transform) * point
		// local_normal ← local_normal_at(shape, local_point)
		// world_normal ← transpose(inverse(shape.transform)) * local_normal
		// world_normal.w ← 0
		// return normalize(world_normal)

		// function local_normal_at(cylinder, point)
		// # compute the square of the distance from the y axis
		// dist ← point.x² + point.z²
		// if dist < 1 and point.y >= cylinder.maximum - EPSILON
		// return vector(0, 1, 0)
		// else if dist < 1 and point.y <= cylinder.minimum + EPSILON
		// return vector(0, -1, 0)
		// else
		// return vector(point.x, 0, point.z)
		// end if
		// end function
		mat_vec_multiply(&object_normal, &shape->inv_transf, itx_point);
		double	distance = object_normal.x * object_normal.x + object_normal.z * object_normal.z;
		if (distance < 1 && object_normal.y >= (shape->height / 2) - EPSILON)
		{
			object_normal.x = 0;
			object_normal.y = 1;
			object_normal.z = 0;
			object_normal.w = 0;
			// mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
			// world_normal.w = 0;
			// normalize_vec(&world_normal);
			// print_vector(&world_normal);
			return (object_normal);
		}
		if (distance < 1 && object_normal.y <= -(shape->height / 2) + EPSILON)
		{
			object_normal.x = 0;
			object_normal.y = -1;
			object_normal.z = 0;
			object_normal.w = 0;
			// mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
			// world_normal.w = 0;
			// normalize_vec(&world_normal);
			// print_vector(&world_normal);
			return (object_normal);
		}
		object_normal.y = 0;
		object_normal.w = 0;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
	}
	return (world_normal);
}

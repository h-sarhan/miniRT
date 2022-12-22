/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:07:05 by mkhan             #+#    #+#             */
/*   Updated: 2022/12/22 19:24:34 by hsarhan          ###   ########.fr       */
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
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);
}

bool	check_cylinder_caps(t_ray *ray, t_shape *shape, t_intersections *xs)
{
	bool	intersected;

	double cylinder_bottom = (shape->height / 2);
	double cylinder_top = -(shape->height / 2);
	intersected = false;
	if (fabs(ray->direction.y) > EPSILON)
	{
		double t;
		t = (cylinder_top - ray->origin.y) / ray->direction.y;
		if (within_cylinder_radius(ray, t))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		t = (cylinder_bottom - ray->origin.y) / ray->direction.y;
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
		if (fabs(transf_ray.direction.y) < 0.00001)
			return (false);
		xs->arr[xs->count].time = -transf_ray.origin.y / transf_ray.direction.y;
		xs->arr[xs->count].shape = shape;
		xs->count++;
	}
	if (shape->type == CYLINDER)
	{
		bool	intersected = check_cylinder_caps(&transf_ray, shape, xs);
		a = transf_ray.direction.x * transf_ray.direction.x + transf_ray.direction.z * transf_ray.direction.z;
		if (fabs(a) < 0.00001)
		{
			return (intersected);
		}
		b = 2 * transf_ray.direction.x * transf_ray.origin.x + 2 * transf_ray.direction.z * transf_ray.origin.z;
		c = transf_ray.origin.x * transf_ray.origin.x + transf_ray.origin.z * transf_ray.origin.z - 1;
		discriminant = b * b - 4 * a * c;
		if (discriminant < 0)
		{
			return (intersected);
		}
		discriminant = sqrt(discriminant);
		double t0 = (-b - discriminant) / (a * 2);
		double t1 = (-b + discriminant) / (a * 2);
		if (t0 > t1)
		{
			double temp = t0;
			t0 = t1;
			t1 = temp;
		}
		
		double	y0 = transf_ray.origin.y + t0 * transf_ray.direction.y;
		if (y0 > (-shape->height / 2) && y0 < (shape->height / 2))
		{
			xs->arr[xs->count].time = t0;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		double	y1 = transf_ray.origin.y + t1 * transf_ray.direction.y;
		if (y1 > (-shape->height / 2) && y1 < (shape->height / 2))
		{
			xs->arr[xs->count].time = t1;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		return (intersected);
	}
	else if (shape->type == CUBE)
	{
		intersect_cube(shape, &transf_ray, xs);
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

t_vector	cylinder_normal(const t_shape *shape, t_vector *point)
{
	(void)shape;
	double		distance = point->x * point->x + point->z * point->z;
	double		cylinder_bottom = (shape->height / 2);
	double		cylinder_top = -(shape->height / 2);
	t_vector	normal;
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
		return (normal);
	}
}

t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	object_normal;
	t_vector	world_normal;
	double		maxc;

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
		t_vector	local_point;
		mat_vec_multiply(&local_point, &shape->inv_transf, itx_point);
		local_point.w = 0;
		t_vector	local_normal;
		local_normal = cylinder_normal(shape, &local_point);
		// Calculate this
		t_vector	world_normal;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &local_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
		return (world_normal);
	}
	if (shape->type == CUBE)
	{
		mat_vec_multiply(&object_normal, &shape->inv_transf, itx_point);
		object_normal.w = 0;
		maxc = find_max(fabs(object_normal.x), fabs(object_normal.y), fabs(object_normal.z));
		// if (maxc == fabs(object_normal.x))
		// {
		// 	//
		// }
		// else if (maxc == fabs(object_normal.y))
		// {
		// 	//
		// }
		// else if (maxc == fabs(object_normal.z))
		// {
		// 	//
		// }
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
			
	}
	if (shape->type == CUBE)
	{
		mat_vec_multiply(&object_normal, &shape->inv_transf, itx_point);
		object_normal.w = 0;
		maxc = find_max(fabs(object_normal.x), fabs(object_normal.y), fabs(object_normal.z));
		// if (maxc == fabs(object_normal.x))
		// {
		// 	//
		// }
		// else if (maxc == fabs(object_normal.y))
		// {
		// 	//
		// }
		// else if (maxc == fabs(object_normal.z))
		// {
		// 	//
		// }
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
			
	}
	return (world_normal);
}

double	find_max(double n1, double n2, double n3)
{
	if (n1 >= n2 && n1 >= n3)
		return (n1);
	if (n2 >= n1 && n2 >= n3)
		return (n2);
	return (n3);
}

void	intersect_cube(t_shape *shape, t_ray *ray, t_intersections *xs)
{
	double	xtmin;
	double	xtmax;
	double	ytmin;
	double	ytmax;
	double	ztmin;
	double	ztmax;
	double	tmin;
	double	tmax;
	
	check_axis(&xtmin, &xtmax, ray->origin.x, ray->direction.x);
	check_axis(&ytmin, &ytmax, ray->origin.y, ray->direction.y);
	check_axis(&ztmin, &ztmax, ray->origin.z, ray->direction.z);
	tmin = find_max(xtmin, ytmin, ztmin);
	tmax = find_max(xtmax, ytmax, ztmax);
	if (tmin > tmax)
		return ;
	xs->arr[xs->count].time = tmin;	
	xs->arr[xs->count].shape = shape;	
	xs->arr[xs->count + 1].time = tmax;	
	xs->arr[xs->count + 1].shape = shape;
	xs->count += 2;	
}

void	check_axis(double *t_min, double *t_max, double origin, double direction)
{
	double		tmin_numerator;
	double		tmax_numerator;
	// double		dir_mag;
	
	tmin_numerator = (-1 - origin);
	tmax_numerator = 1 - origin;
	if (fabs(direction) >= EPSILON)
	{
		*t_min = tmin_numerator / direction;
		*t_max = tmax_numerator / direction;
	}
	else
	{
		*t_min = tmin_numerator * INFINITY;
		*t_max = tmax_numerator * INFINITY;
	}
	if (t_min > t_max)
		ft_swapd(t_min, t_max);		
}
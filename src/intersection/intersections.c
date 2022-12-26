/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:07:05 by mkhan             #+#    #+#             */
/*   Updated: 2022/12/26 20:39:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ray_position(t_vector *pos, const t_ray *ray, float time)
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
	float			distance;
	t_ray			ray;
	t_intersections	arr;
	int				i;
	t_intersection	*intersection;

	sub_vec(&ray.direction, &scene->lights[light_idx].position, itx_point);
	distance = vec_magnitude(&ray.direction);
	scale_vec(&ray.direction, &ray.direction, 1 / distance);
	ray.origin = *itx_point;
	i = -1;
	arr.count = 0;
	while (++i < scene->count.shapes)
		intersect(&scene->shapes[i], &ray, &arr);
	intersection = hit_skip_transparent(&arr);
	if (intersection && intersection->time < distance)
		return (true);
	return (false);
}

bool	intersect_sphere_fast(const t_ray *ray, t_intersections *xs, t_shape *sphere)
{
	t_vector	oc;
	sub_vec(&oc, &ray->origin, &sphere->origin);
	float	b = dot_product(&oc, &ray->direction);
	float	c = dot_product(&oc, &oc) - sphere->props.radius_squared;
	float	h = b * b - c;
	if (h < 0.0)
		return (false);
	h = sqrt(h);
	xs->arr[xs->count].time = - b - h;
	xs->arr[xs->count].shape = sphere;
	xs->arr[xs->count + 1].time = -b + h;
	xs->arr[xs->count + 1].shape = sphere;
	xs->count += 2;
	return (true);
}

bool	intersect_sphere(t_ray *transf_ray, t_intersections *xs, t_shape *sphere)
{
	float	a;
	float	b;
	float	c;
	float	discriminant;
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
	xs->arr[xs->count].shape = sphere;
	xs->arr[xs->count + 1].time = (b + discriminant) / a;
	xs->arr[xs->count + 1].shape = sphere;
	xs->count += 2;
	return (true);
}
bool	within_cylinder_radius(t_ray *ray, float t)
{
	float x = ray->origin.x + ray->direction.x * t;
	float z = ray->origin.z + ray->direction.z * t;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);
}

bool	check_cylinder_caps(t_ray *ray, t_shape *shape, t_intersections *xs)
{
	bool	intersected;

	float cylinder_bottom = (shape->props.height / 2);
	float cylinder_top = -(shape->props.height / 2);
	intersected = false;
	if (fabs(ray->direction.y) > EPSILON)
	{
		float t;
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

bool	within_cone_radius(t_ray *ray, float t, float cone_val)
{
	float x = ray->origin.x + ray->direction.x * t;
	float z = ray->origin.z + ray->direction.z * t;
	if (x * x + z * z <= fabs(cone_val) * fabs(cone_val))
		return (true);
	return (false);
}

bool	check_cone_caps(t_ray *ray, t_shape *shape, t_intersections *xs)
{
	bool	intersected;

	float cone_top= (0);
	float cone_bottom = -(shape->props.height / 2);
	intersected = false;
	if (fabs(ray->direction.y) > EPSILON)
	{
		float t;
		t = (cone_top - ray->origin.y) / ray->direction.y;
		if (within_cone_radius(ray, t, cone_top))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		t = (cone_bottom - ray->origin.y) / ray->direction.y;
		if (within_cone_radius(ray, t, cone_bottom))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
	}
	return (intersected);
}


t_intersection	*hit(t_intersections *xs)
{
	float	min_time;
	int		i;
	int		idx;

	i = 0;
	idx = 0;
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

t_intersection	*hit_skip_transparent(t_intersections *xs)
{
	float	min_time;
	int		i;
	int		idx;

	i = 0;
	min_time = INFINITY;
	idx = 0;
	while (i < xs->count)
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time && xs->arr[i].shape->props.transparency <= 0.5)
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

t_intersection	*hit_sorted(t_intersections *xs)
{
	int		i;

	i = 0;
	while (i < xs->count)
	{
		if (xs->arr[i].time >= 0)
			return (&xs->arr[i]);
		i++;
	}
	return (NULL);
}

t_vector	cylinder_normal(const t_shape *shape, t_vector *point)
{
	(void)shape;
	float		distance = point->x * point->x + point->z * point->z;
	float		cylinder_bottom = (shape->props.height / 2);
	float		cylinder_top = -(shape->props.height / 2);
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

t_vector	cone_normal(const t_shape *shape, t_vector *point)
{
	float		distance = point->x * point->x + point->z * point->z;
	float		radius = fabs(point->y) * fabs(point->y);
	float		cone_bottom = (0);
	float		cone_top = -(shape->props.height / 2);
	t_vector	normal;
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
		return (normal);
	}
}

t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	object_normal;
	t_vector	world_normal;
	float		maxc;

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
		normalize_vec(&local_normal);
		// Calculate this
		t_vector	world_normal;
		mat_vec_multiply(&world_normal, &shape->norm_transf, &local_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
		return (world_normal);
	}
	if (shape->type == CONE)
	{
		t_vector	local_point;
		mat_vec_multiply(&local_point, &shape->inv_transf, itx_point);
		local_point.w = 0;
		t_vector	local_normal;
		local_normal = cone_normal(shape, &local_point);
		normalize_vec(&local_normal);
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
		maxc = max3(fabs(object_normal.x), fabs(object_normal.y), fabs(object_normal.z));
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
		mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
		world_normal.w = 0;
		normalize_vec(&world_normal);
	}
	return (world_normal);
}

float	max3(float n1, float n2, float n3)
{
	if (n1 >= n2 && n1 >= n3)
		return (n1);
	if (n2 >= n1 && n2 >= n3)
		return (n2);
	return (n3);
}

float	min3(float n1, float n2, float n3)
{
	if (n1 <= n2 && n1 <= n3)
		return (n1);
	if (n2 <= n1 && n2 <= n3)
		return (n2);
	return (n3);
}

bool	intersect_cube(t_shape *shape, t_ray *ray, t_intersections *xs)
{
	float	xtmin;
	float	xtmax;
	float	ytmin;
	float	ytmax;
	float	ztmin;
	float	ztmax;
	float	tmin;
	float	tmax;
	
	check_axis(&xtmin, &xtmax, ray->origin.x, ray->direction.x);
	check_axis(&ytmin, &ytmax, ray->origin.y, ray->direction.y);
	check_axis(&ztmin, &ztmax, ray->origin.z, ray->direction.z);
	tmin = max3(xtmin, ytmin, ztmin);
	tmax = min3(xtmax, ytmax, ztmax);
	if (tmin > tmax)
		return (false);
	xs->arr[xs->count].time = tmin;	
	xs->arr[xs->count].shape = shape;	
	xs->arr[xs->count + 1].time = tmax;	
	xs->arr[xs->count + 1].shape = shape;
	xs->count += 2;	
	return (true);
}

void	check_axis(float *t_min, float *t_max, float origin, float direction)
{
	float		tmin_numerator;
	float		tmax_numerator;
	// float		dir_mag;
	
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
	if (*t_min > *t_max)
		ft_swapd(t_min, t_max);
}

bool	intersect(t_shape *shape, const t_ray *ray, t_intersections *xs)
{
	t_ray	transf_ray;
	float	a;
	float	b;
	float	c;
	float	discriminant;

	if (shape->type == SPHERE)
	{
		return (intersect_sphere_fast(ray, xs, shape));
	}
	if (shape->type == PLANE)
	{
		double	denom = dot_product(&ray->direction, &shape->orientation);
		if (fabs(denom) < 0.00001)
			return (false);
		xs->arr[xs->count].time = -(dot_product(&ray->origin, &shape->orientation) \
			- shape->props.distance_from_origin) / denom;
		xs->arr[xs->count].shape = shape;
		xs->count++;
		return (true);
	}
	transform_ray(&transf_ray, ray, shape);
	// if (shape->type == SPHERE)
	// {
	// 	return (intersect_sphere(&transf_ray, xs, shape));
	// }
	// if (shape->type == PLANE)
	// {
	// 	if (fabs(transf_ray.direction.y) < 0.00001)
	// 		return (false);
	// 	xs->arr[xs->count].time = -transf_ray.origin.y / transf_ray.direction.y;
	// 	xs->arr[xs->count].shape = shape;
	// 	xs->count++;
	// 	return (true);
	// }
	if (shape->type == CYLINDER)
	{
		bool	intersected = check_cylinder_caps(&transf_ray, shape, xs);
		a = transf_ray.direction.x * transf_ray.direction.x + transf_ray.direction.z * transf_ray.direction.z;
		if (fabs(a) < 0.0001)
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
		a *= 2;
		b *= -1;
		discriminant = sqrt(discriminant);
		float t0 = (b - discriminant) / (a);
		float t1 = (b + discriminant) / (a);
		if (t0 > t1)
		{
			ft_swapd(&t0, &t1);
		}
		float	y0 = transf_ray.origin.y + t0 * transf_ray.direction.y;
		if (y0 > (-shape->props.height / 2) && y0 < (shape->props.height / 2))
		{
			xs->arr[xs->count].time = t0;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		float	y1 = transf_ray.origin.y + t1 * transf_ray.direction.y;
		if (y1 > (-shape->props.height / 2) && y1 < (shape->props.height / 2))
		{
			xs->arr[xs->count].time = t1;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		return (intersected);
	}
	if (shape->type == CONE)
	{
		bool	intersected = check_cone_caps(&transf_ray, shape, xs);
		a = transf_ray.direction.x * transf_ray.direction.x - transf_ray.direction.y * transf_ray.direction.y + transf_ray.direction.z * transf_ray.direction.z;
		b = 2 * transf_ray.direction.x * transf_ray.origin.x - 2 * transf_ray.direction.y * transf_ray.origin.y + 2 * transf_ray.direction.z * transf_ray.origin.z;
		c = transf_ray.origin.x * transf_ray.origin.x - transf_ray.origin.y * transf_ray.origin.y + transf_ray.origin.z * transf_ray.origin.z;
		discriminant = b * b - 4 * a * c;
		if (discriminant < 0)
			return (intersected);
		if (fabs(a) < 0.000001 && fabs(b) > 0.1)
		{
			xs->arr[xs->count].time = - c / (2 * b);
			xs->arr[xs->count].shape = shape;
			xs->count++;
			return (intersected);
		}
		// if (fabs(a) < EPSILON)
		// 	return (intersected);
		discriminant = sqrt(discriminant);
		float t0 = (-b - discriminant) / (a * 2);
		float t1 = (-b + discriminant) / (a * 2);
		if (t0 > t1)
		{
			ft_swapd(&t0, &t1);
		}
		
		float	y0 = transf_ray.origin.y + t0 * transf_ray.direction.y;
		if (y0 > (-shape->props.height / 2) && y0 < (0))
		{
			xs->arr[xs->count].time = t0;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		float	y1 = transf_ray.origin.y + t1 * transf_ray.direction.y;
		if (y1 > (-shape->props.height / 2) && y1 < (0))
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
		return (intersect_cube(shape, &transf_ray, xs));
	}
	return (true);
}
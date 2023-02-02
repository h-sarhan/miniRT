/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 15:56:36 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/02 19:37:36 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static bool	within_cone_radius(const t_ray *ray, float t, float cone_val)
{
	float	x;
	float	z;

	x = ray->origin.x + ray->dir.x * t;
	z = ray->origin.z + ray->dir.z * t;
	if ((x * x + z * z) <= (fabs(cone_val) * fabs(cone_val)))
		return (true);
	return (false);
}

static bool	check_cone_caps(const t_ray *ray, t_shape *shape,
		t_intersections *xs)
{
	bool	intersected;
	float	t;

	intersected = false;
	if (fabs(ray->dir.y) > EPSILON)
	{
		t = (shape->props.height / 2 - ray->origin.y) / ray->dir.y;
		if (within_cone_radius(ray, t, shape->props.height / 2))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		t = -ray->origin.y / ray->dir.y;
		if (within_cone_radius(ray, t, 0))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
	}
	return (intersected);
}

static bool	add_cone_intersections(t_shape *shape, const t_ray *ray,
	t_intersections *xs, float *ts)
{
	float	y0;
	float	y1;
	bool	intersected;

	intersected = false;
	if (ts[0] > ts[1])
		ft_swapd(&ts[0], &ts[1]);
	y0 = ray->origin.y + ts[0] * ray->dir.y;
	if (y0 > 0 && y0 < ((shape->props.height / 2)))
	{
		xs->arr[xs->count].time = ts[0];
		xs->arr[xs->count].shape = shape;
		xs->count++;
		intersected = true;
	}
	y1 = ray->origin.y + ts[1] * ray->dir.y;
	if (y1 > 0 && y1 < ((shape->props.height / 2)))
	{
		xs->arr[xs->count].time = ts[1];
		xs->arr[xs->count].shape = shape;
		xs->count++;
		intersected = true;
	}
	return (intersected);
}

static float	get_cone_discriminant(const t_ray *ray, float *abc)
{
	float	discriminant;

	abc[0] = ray->dir.x * ray->dir.x - ray->dir.y * ray->dir.y \
	+ ray->dir.z * ray->dir.z;
	abc[1] = 2 * ray->dir.x * ray->origin.x - 2 * ray->dir.y * ray->origin.y \
		+ 2 * ray->dir.z * ray->origin.z;
	abc[2] = ray->origin.x * ray->origin.x - ray->origin.y * ray->origin.y \
		+ ray->origin.z * ray->origin.z;
	discriminant = abc[1] * abc[1] - 4 * abc[0] * (ray->origin.x * \
	ray->origin.x - ray->origin.y * ray->origin.y + ray->origin.z * \
	ray->origin.z);
	return (discriminant);
}

bool	intersect_cone(const t_ray *ray, t_shape *shape, t_intersections *xs)
{
	bool	intersected;
	float	abc[3];
	float	discriminant;
	float	ts[2];

	intersected = check_cone_caps(ray, shape, xs);
	discriminant = get_cone_discriminant(ray, abc);
	if (discriminant < 0)
		return (intersected);
	if (fabs(abc[0]) < 0.000001 && fabs(abc[1]) > 0.1)
	{
		xs->arr[xs->count].time = -abc[2] / (2 * abc[1]);
		xs->arr[xs->count++].shape = shape;
		return (intersected);
	}
	discriminant = sqrt(discriminant);
	abc[1] *= -1;
	abc[0] *= 2;
	ts[0] = (abc[1] - discriminant) / (abc[0]);
	ts[1] = (abc[1] + discriminant) / (abc[0]);
	if (add_cone_intersections(shape, ray, xs, ts) == true)
		intersected = true;
	return (intersected);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:23:32 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/20 18:55:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	prepare_computations(t_scene *scene, t_intersection *intersection,
			t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(scene, intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->dir);
	intersection->eye.w = 0;
	intersection->inside = false;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect_vector(&intersection->reflect_vec, &ray->dir,
		&intersection->normal);
}

t_intersection	*hit(t_intersections *xs)
{
	double	min_time;
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

void	ray_from_cam(t_ray *ray, const t_camera *cam, double x, double y)
{
	double		world_x;
	double		world_y;
	t_vector	pixel;
	t_vector	world_point;
	t_vector	center;

	world_x = cam->half_width - (x) * cam->pixel_size;
	world_y = cam->half_height - (y) * cam->pixel_size;
	world_point.x = world_x;
	world_point.y = world_y;
	world_point.z = -1;
	world_point.w = 1;
	mat_vec_multiply(&pixel, &cam->inv_trans, &world_point);
	ft_bzero(&center, sizeof(t_vector));
	center.w = 1;
	mat_vec_multiply(&ray->origin, &cam->inv_trans, &center);
	sub_vec(&ray->dir, &pixel, &ray->origin);
	ray->dir.w = 0;
	normalize_vec(&ray->dir);
}

// void	ray_from_cam(t_ray *ray, const t_camera *cam, int x, int y)
// {
// 	double		world_x;
// 	double		world_y;
// 	t_vector	pixel;
// 	t_vector	world_point;
// 	t_vector	center;

// 	world_x = cam->half_width - (x + 0.5) * cam->pixel_size;
// 	world_y = cam->half_height - (y + 0.5) * cam->pixel_size;
// 	world_point.x = world_x;
// 	world_point.y = world_y;
// 	world_point.z = -1;
// 	world_point.w = 1;
// 	mat_vec_multiply(&pixel, &cam->inv_trans, &world_point);
// 	ft_bzero(&center, sizeof(t_vector));
// 	center.w = 1;
// 	mat_vec_multiply(&ray->origin, &cam->inv_trans, &center);
// 	sub_vec(&ray->direction, &pixel, &ray->origin);
// 	ray->direction.w = 0;
// 	normalize_vec(&ray->direction);
// }

void	ray_position(t_vector *pos, const t_ray *ray, double time)
{
	pos->x = ray->dir.x * time + ray->origin.x;
	pos->y = ray->dir.y * time + ray->origin.y;
	pos->z = ray->dir.z * time + ray->origin.z;
	pos->w = 1;
}

void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
	const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf,
		&ray->origin);
	mat_vec_multiply(&transformed_ray->dir, &shape->inv_transf,
		&ray->dir);
}

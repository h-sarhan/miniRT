/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:07:05 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/22 20:17:37 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	my_mlx_pixel_put(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bytes_per_pixel));
	*(unsigned int*)dst = color;
}

void	position(t_vector *pos, t_ray *ray, float time)
{
	scale_vec(pos, &ray->direction, time);
	add_vec(pos, pos, &ray->origin);
}

void	transform(t_ray *ray, t_shape *shape)
{
	mat_vec_multiply(&ray->origin, &shape->inv_transf, &ray->origin);
	mat_vec_multiply(&ray->direction, &shape->inv_transf, &ray->direction);
}

bool	intersect(t_shape *shape, t_ray *ray, t_intersections *xs)
{
	float		a;
	float		b;
	float		c;
	float		discriminant;
	t_vector	sphere_to_ray;
	t_vector	center;

	center.x = 0;
	center.y = 0;
	center.z = 0;
	center.w = 1;
	transform(ray, shape);
	sub_vec(&sphere_to_ray, &ray->origin, &center);
	a = dot_product(&ray->direction, &ray->direction);
	b = 2 * dot_product(&ray->direction, &sphere_to_ray);
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

t_intersect	*hit(t_intersections *xs)
{
	float	min_time;
	int		i;
	int 	idx;

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

t_vector	normal_at(t_shape *shape, t_vector *intersection_point)
{
	t_vector origin;
	t_vector object_point;
	t_vector object_normal;
	t_vector world_normal;
	
	// Object point calculation
	mat_vec_multiply(&object_point, &shape->inv_transf, intersection_point);
	
	// Object normal calculation
	origin.x = 0;
	origin.y = 0;
	origin.z = 0;
	origin.w = 1;
	sub_vec(&object_normal, &object_normal, &origin);
	
	// World normal calculation
	mat_vec_multiply(&world_normal, &shape->norm_transf, &object_normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return(world_normal);
}

void draw_scene(t_scene *scene)
{
	float	wall_z;
	float	wall_size;
	float	pixel_size;
	float	half;
	float	world_y;
	float	world_x;
	t_vector position;
	t_vector ray_origin;
	t_ray	ray;
	t_intersections arr;
	t_intersect intersect_arr[100];
	int		x;
	int		y;
	t_mlx	*mlx;

	mlx = scene->mlx;
	x = 0;
	y = 0;
	wall_z = 10;
	wall_size = 7.0;
	// ft_bzero(mlx->addr, mlx->bytes_per_pixel * scene->win_h * scene->win_w);
	ray_origin.x = 0;
	ray_origin.y = 0;
	ray_origin.z = -5;
	ray_origin.w = 1;
	pixel_size = wall_size / (float)scene->win_h;
	half = wall_size / 2;
	arr.arr = intersect_arr;
	arr.count = 0;
	int pixel = 0;
	TICK(render);
	while (y < scene->win_h)
	{
		world_y = half - (pixel_size * y);
		x = 0;
		while (x < scene->win_h)
		{
			world_x = (half * -1) + (pixel_size * x);
			position.x = world_x;
			position.y = world_y;
			position.z = wall_z;
			position.w = 1;
			ray.origin = ray_origin;
			sub_vec(&ray.direction, &position, &ray_origin);
			normalize_vec(&ray.direction);
			arr.count = 0;
			intersect(&scene->shapes[0], &ray, &arr);
			t_intersect *intersection  = hit(&arr);
			if (intersection != NULL)
			{
				*(unsigned int *)(mlx->addr + pixel) = intersection->shape->mlx_color;
				// my_mlx_pixel_put(mlx, x, y, intersection->shape->mlx_color);
			}
			else
			{
				*(unsigned int *)(mlx->addr + pixel) = 0;
			}
			pixel += mlx->bytes_per_pixel;
			x++;
		}
		y++;
	}
	TOCK(render);
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->img, 0, 0);
}
